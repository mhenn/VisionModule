document.addEventListener("keydown", keyDownHandler, false);


function keyDownHandler(event) {
    
    var base = "processmotion&mode=";    

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
}



