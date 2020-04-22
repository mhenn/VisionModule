document.addEventListener("keydown", keyDownHandler, false);


function keyDownHandler(event) {
    
    var base = "processmotion&mode=";    
    
    switch(event.keyCode){
    //camera control
    // yaw
    case 39: 
       SendCommand(base + "yaw&direction=pos");
    break;
    case 37: 
       SendCommand(base + "yaw&direction=neg");
    break;
    //pitch
    case 40: 
       SendCommand(base + "pit&direction=pos");
    break;
    case 38: 
       SendCommand(base + "pit&direction=neg");
    break;
    //car control
    //steering
    case 65: 
       SendCommand(base + "ste&direction=pos");
    break;
    case 68: 
       SendCommand(base + "ste&direction=neg");
    break;
    //speed
    case 87: 
       SendCommand(base + "spe&direction=pos");
    break;
    case 83: 
       SendCommand(base + "spe&direction=neg");
    break;
    } 
/*    
    if(event.keyCode == 39) {
       console.log("plus yaw"); 
       SendCommand(base + "yaw&direction=pos");
    }
    else if(event.keyCode == 37) {
       SendCommand(base + "yaw&direction=neg");
    }
    if(event.keyCode == 40) {
       SendCommand(base + "pit&direction=pos");
    }
    else if(event.keyCode == 38) {
       SendCommand(base + "pit&direction=neg");
    }
*/

}



