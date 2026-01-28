import { useState, useEffect} from 'react'

import './App.css'

function App() {

//react events handler
  const [message, setMessage] = useState("");
  const [value, setValue] = useState<number>(0);

/*

  const switchLed = async (is_on: boolean) => {
    const payload = { is_on } ;
    const webResult = await fetch("/api/toggle-led", {
      method: "POST",
      body: JSON.stringify(payload)

    });                 //if a response is received then we change state of ledOn, react only change the graphics
                        //but dont really control the I/O
    if(!webResult.ok)
    {
      console.error(webResult.statusText);
      return;
    }
    setLedOn(is_on);
  }

  const getLedText = () => {
    return ledOn ? "LED is on" : "LED is off";
  }
*/

//event handlers

  const getHelloWorld = async () =>     //js function for texting purpose
  {
                                                                              //before it was : 
    const webResult = await fetch("/api/hello-world");   //const webResult = await fetch("http://my-esp32.local/api/hello-world"); 
    const MyText = await webResult.text();
    setMessage(MyText);
    console.log(MyText);
  }



  const getWebSocketUrl = (suffix: string) =>
  {
    const l = window.location;
    return ((l.protocol === "https:" ? "wss://":  "ws://") + l.host + l.pathname + suffix);
  }


  //server creates JSON object which will be read from the websocket

  const webSocket = () => 
  {
    const socket = new WebSocket(getWebSocketUrl("ws"));    //create new websocket
    socket.onopen = () =>        //on opened websocket
    {
      socket.send("Hello esp32");
    }
    socket.onmessage = (event) =>       //server send data to client
    {
      console.log(event.data);    //event.data is data the server received
      try{
        //const attemptedBtnState = JSON.parse(event.data);
        //setValue(attemptedBtnState.btn_state);       //btn_state is built from server with cJSON
      }
      finally {
        
      }
    }
    socket.onerror = (err) =>     //catch errors on ws
    {
      console.error(err);
    };
    socket.onclose = (event) =>     //ws get closed
    {
      console.log(event);
    }
  }
  


  const handleChange = async (e: React.ChangeEvent<HTMLInputElement>) => 
  {
    const val = Number(e.target.value);
    setValue(val);

    // invia richiesta HTTP GET
    const webResult = await fetch(`/api/setBrightness?value=${val}`)
    const MyText = await webResult.text();
    console.log(MyText);
  };

  const getLedOn = async () =>   
  {
    const webResult = await fetch("/api/LedOn");    
    const MyText = await webResult.text();
    
    console.log(MyText);
  }

  const getLedOff = async () =>   
  {
    const webResult = await fetch("/api/LedOff");    
    const MyText = await webResult.text();
    
    console.log(MyText);
  }

  //react logic
  useEffect
  (
    () => {
       getHelloWorld();      //1. callback func
       webSocket();
      },         
    []                //2. when to update or run this func [] = run only once when component is rendered
  )
 







  return (
    <>
     
      <h1>hello world</h1>
      <h1>
        {message}
      </h1>

      <button onClick={getLedOn}>
        Turn on LED!
      </button>

      <button onClick={getLedOff}>
        Turn off LED!
      </button>

      <div>
        <h1>Controllo LED PWM</h1>
        <input
          type="range"
          min="0"
          max="100"
          value={value}
          onInput={handleChange} // onInput più reattivo per slider
        />
        <p>Valore: {value}</p>
      </div>



  
    </>
  )
}

export default App
