import Reading from "./Reading";
import { useState, useEffect, useRef } from "react";

import axios from "axios";

function Sensor({ id, name, state }) {
  const [data, setData] = useState([]);
  const [status, setStatus] = useState(state)

  function turnmeon(){
    if(status == 'ON') setStatus('OFF')
    else setStatus('ON')
    axios.get(`http://localhost:8085/change_sensor_state/${id}`)
  }


  useEffect(() => {
    axios.get(`http://localhost:8085/readings/${id}`).then((result) => {
      setData(result.data);
    });
  }, []);

  return (
    <div className="m-1 mt-2">
      <div className="p-1 rounded border-4">
        <div className="text-xl font-semibold inline ml-4">{name}</div>
        <div className="text-xs inline"> #{id}</div>
        {status == "ON" && (
          <div className="rounded bg-green-500 w-2 h-2 inline-block ml-4 border-1"></div>
        )}
        {status == "OFF" && (
          <div className="rounded bg-red-400 w-2 h-2 inline-block ml-4 border-1"></div>
        )}
      </div>

      <div className="border border-t-0 -mt-1 p-1">
        {data.map(function (object, i) {
          return (
            <Reading
              key={i}
              id={object.id_leitura}
              value={object.dados}
              timestamp={object.timestamp}
            />
          );
        })}

        <div className="pt-3">
          <input
            className="text-white text-sm font-bold bg-green-300 p-1.5 rounded mr-5"
            type="button"
            value="Start"
            onClick={turnmeon}
          />
          <input
            className="text-white text-sm font-bold bg-red-400 p-1.5 rounded mr-5"
            type="button"
            value="Stop"
            onClick={turnmeon}
          />
        </div>
      </div>
    </div>
  );
}

export default Sensor;
