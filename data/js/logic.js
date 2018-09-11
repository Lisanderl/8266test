document.addEventListener('DOMContentLoaded', function() {
	

  function getSteps() {
      return  document.getElementById("steps").value;
  }

    function sendAction( name, steps, actionId ) {
        var xhr = new XMLHttpRequest();
        xhr.open('POST', '/action');
        xhr.setRequestHeader('Content-Type', 'application/json');
        xhr.send(JSON.stringify({"name": name, "steps": steps, "actionId": actionId}));

    }

    //logic for buttons on UI
    document.getElementById("straight").onclick = function() {
        sendAction(this.id, getSteps(), 1);
        console.log('clicked ' + this.id + 'steps ' + getSteps());
    };

    document.getElementById("back").onclick = function() {
        sendAction(this.id, getSteps(), 2);
        console.log('clicked ' + this.id);
    };

    document.getElementById("left").onclick = function() {
        sendAction(this.id, getSteps(), 3);
        console.log('clicked ' + this.id);
    };

    document.getElementById("right").onclick = function() {
        sendAction(this.id, getSteps(), 4);
        console.log('clicked ' + this.id);
    };

    document.getElementById("leftRotate").onclick = function() {
        sendAction(this.id, getSteps(), 3);
        console.log('clicked ' + this.id);
    };

    document.getElementById("rightRotate").onclick = function() {
        sendAction(this.id, getSteps(), 4);
        console.log('clicked ' + this.id);
    };

    document.getElementById("sliderValue").textContent = getSteps();
    document.getElementById("steps").onchange = function() {
        document.getElementById("sliderValue").textContent = getSteps();
         };

});