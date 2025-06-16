import { useState, useEffect } from "react";

import System from "../components/System";
import axios from "axios";

function SystemPage() {
  const [systems, setSystems] = useState([]);

  useEffect(function () {
    axios.get("http://localhost:8085/ss").then((result) => {
      setSystems(result.data);
    });
  });

  return (
    <div className="max-w-md m-auto">
      {systems.map(function (object, i) {
        return (
          <System key={i} id={object.id_SS} name={object.nome_sistema_sensor} state={object.estado} />
        );
      })}
    </div>
  );
}

export default SystemPage;
