#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/program_options.hpp>

#include "videostream.h"
#include "httpd.h"
#include <arpa/inet.h>

using namespace std;

namespace po = boost::program_options;

globals VideoStream::global;

int 
main( int argc, char ** argv )
{
  string config_file;
  string device_name;
  string driver;

  string input("default");
  string standard("default");

  int bayer = 0;

  unsigned int width = 0;
  unsigned int height = 0;
  unsigned int depth = 32;
  unsigned int numBuffers = 2;
  unsigned int fps = 10;

  unsigned int http_port;
  string http_addr;
  string docroot;

  try 
    {
      // Declare a group of options that will be 
      // allowed only on command line
      po::options_description commandLineOnlyOptions("Command Line Options");
      commandLineOnlyOptions.add_options()
	("version,v", "print version string")
	("help", "produce help message")    
	("config,c", po::value<string>( & config_file )->default_value("vision_module.cfg"), "config file name")
	;
      
      po::options_description generalOptions("General Options");
      generalOptions.add_options()
	("driver,u", po::value<string>(& driver)->default_value("v4l2"), "driver type [v4l2,v4l,bayer,file]")
	("device,d", po::value<string>(& device_name)->default_value("/dev/video0"), "device name")
	("width,w", po::value<unsigned int>( & width )->default_value(320),"width")
	("height,h", po::value<unsigned int>( & height )->default_value(240),"height")
	("depth", po::value<unsigned int>( & depth )->default_value(24),"depth")
	;

      po::options_description libwtOptions("Http Server Options");
      libwtOptions.add_options()
	("http-port", po::value<unsigned int>( & http_port )->default_value(8080)->required(),"http port number")
	("http-addr", po::value<string>(& http_addr)->default_value("0.0.0.0")->required(),"http address")
	("docroot", po::value<string>(& docroot)->default_value("www/")->required(),"http document root");
	 
      po::options_description commandLineOptions;
      commandLineOptions.add(commandLineOnlyOptions).add(generalOptions).add(libwtOptions);

      po::variables_map vm;
      po::store(po::parse_command_line(argc,argv,commandLineOptions),vm);
      po::notify(vm);

      po::options_description configFileOptions;
      configFileOptions.add(generalOptions).add(libwtOptions);

      ifstream ifs( config_file.c_str() );
      po::store(po::parse_config_file(ifs, configFileOptions), vm );
      po::notify(vm);

      if ( vm.count("help") )
	{
	  std::cout << commandLineOptions << "\n";
	  return 1;
	}

      if (vm.count("version") )
	{
	  cout << "Version 0.0.0 Thu Dec  6 02:25:47 CST 2012" << endl;
	  return 1;
	}

      if ( device_name.substr(0, 10) ==  "/dev/video" )
	{
	  driver = "V4L2";
	}
      else if ( device_name.substr(0, 5) ==  "file:" )
	{
	  driver = "File";
	  device_name = device_name.substr( 5, device_name.length() );
	}
      else if ( device_name.substr(0, 11) ==  "/dev/dc1394" )
	{
	  driver = "DC1394";
	}
      else
	{
	 cerr << "Unable to determine video capture device driver " << device_name << endl;
	  return 1;
	}

#if defined(DEBUG)
      cout << "device " << device_name << ", driver " << driver << ", width " << width << ", height " << height << endl;
#endif

      VideoStream * video = new VideoStream( driver, device_name, input, standard, fps, width, height, depth, numBuffers );

      // TODO Auto-generated constructor stub
      VideoStream::global.buf = ( uint8_t *)malloc( width * height * depth );
      VideoStream::global.size = width * height * depth;
  
      if(pthread_mutex_init(& VideoStream::global.db, NULL) != 0)
	{
	  perror("pthread_mutex_init:");
	  exit(EXIT_FAILURE);
	}
      
      if(pthread_cond_init(& VideoStream::global.db_update, NULL) != 0)
	{
	  perror("pthread_cond_init:");
	  exit(EXIT_FAILURE);
	}
      
      if(pthread_mutex_init(& (video->controls_mutex), NULL) != 0)
	{
	  perror("pthread_mutex_init:");
	  exit(EXIT_FAILURE);
	}
      
      video->server.pglobal = & VideoStream::global;
      video->server.conf.http_port = htons( http_port );
      video->server.conf.http_addr = http_addr.c_str();
      video->server.conf.credentials = NULL;
      video->server.conf.docroot = docroot.c_str();
      video->server.conf.nocommands = 0;
      
#if defined(DEBUG)
      cout << "Starting video thread" << endl;
#endif

      pthread_create(&(video->server.threadID), NULL, HTTPD::server_thread, &(video->server));
      pthread_detach(video->server.threadID);

      cout << "Starting video processing thread" << endl;

      pthread_create(&(video->threadID), NULL, (void * (*) ( void *)) video->run_trampoline, static_cast<void *>( video ) );
      pthread_detach(video->threadID);

      for(;;)
	{
	  sleep(100);
	}
    }
  catch( exception & e )
    {
      cerr << e.what() << endl;
      return 1;
    }
}
