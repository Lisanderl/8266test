$(document).ready(function(){
	

  function getSteps() {
      return $( "#steps" ).val();
  };
  
  function sendAction( name, steps, actionId ) {
      $.post( "/action", {"action": { "name": name, "steps": steps, "actionId": actionId }})
          .done(function( data ) {
              alert( "Data Loaded: " + data );
          })
          .fail(function( ) {
              alert( "Somethink went wrong: " + name );
          });
  }

    //logic for buttons on UI
    $( "#straight" ).click(function() {
        sendAction(this.id, getSteps(), 1);
    });

    $( "#back" ).click(function() {
        sendAction(this.id, getSteps(), 2);
    });

    $( "#left" ).click(function() {
        sendAction(this.id, getSteps(), 3);
    });

    $( "#right" ).click(function() {
        sendAction(this.id, getSteps(), 4);
    });

});