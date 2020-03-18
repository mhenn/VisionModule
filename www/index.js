document.addEventListener("keydown", keyDownHandler, false);


function keyDownHandler(event) {
    if(event.keyCode == 39) {
       console.log("plus yaw");
       SendCommand("yaw_plus");
    }
    else if(event.keyCode == 37) {
       SendCommand("yaw_minus");
    }
    if(event.keyCode == 40) {
       SendCommand("pitch_minus");
    }
    else if(event.keyCode == 38) {
       SendCommand("pitch_plus");
    }
}



