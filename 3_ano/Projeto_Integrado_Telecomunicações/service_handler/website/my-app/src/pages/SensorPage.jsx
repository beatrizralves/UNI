import { useLocation } from "react-router-dom";
import { useState, useEffect } from "react";
import axios from "axios";

import Sensor from "../components/Sensor";

function SensorPage() {
  const location = useLocation();
  const myProps = location.state;

  const [sensors, setSensors] = useState([]);

  useEffect(function () {
    axios
      .get(`http://localhost:8085/ss/${myProps.id}/sensors`)
      .then((result) => {
        console.log(result.data[0]);
        setSensors(result.data);
      });
  });

  return (
    <div className="max-w-md m-auto" >
      {sensors.map(function (sensor, i) {
        return (
          <Sensor
            key={i}
            id={sensor.id_sensor}
            name={sensor.nome_sensor}
            state={sensor.estado}
          />
        );
      })}
    </div>
  );
}

export default SensorPage;
