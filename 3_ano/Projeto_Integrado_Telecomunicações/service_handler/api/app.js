const { PrismaClient } = require("@prisma/client");
const prisma = new PrismaClient();

const cors = require("cors");

var net = require("net");

const express = require("express");
const app = express();

const PORT = process.env.PORT || 8085;




app.use(express.json());
app.use(cors());

app.get("/", (req, res) => {
  res.send("API de Projeto de Sensores");
});

app.post("/start_ss/:id_user", async (req, res) => {
  //TODO: create socket connection with service handler through port 2222 and send signal to start new SS associated with id_user
  const client = net.connect({ port: 2222 }, () => {
    console.log('Connected to Service Handler.');
    
    // Data to send to the C program
    const data = {
      message: 'New Sensor System',
      id_user: parseInt(req.params.id_user)
    };
  
    // Convert the data to a string and send it
    const jsonData = JSON.stringify(data);
    client.write(jsonData);
  
    // Close the connection when done
    client.end();
  });
  
  client.on('end', () => {
    console.log('Disconnected from Service Handler.');
  });
  res.send();
});

app.get("/ss/:idSS", async (req, res) => {
  var ss = await prisma.sistema_sensor.findUnique({
    where: {
      id_SS: parseInt(req.params.idSS),
    },
  });
  res.send(ss);
});

app.get("/ss/:idSS/sensors", async (req, res) => {
  var ss = await prisma.sensor.findMany({
    where: {
      id_SS: parseInt(req.params.idSS),
    },
  });

  var arr = [];

  for (const [key, value] of Object.entries(ss)) {
    arr.push(value);
  }

  res.send(ss);
});

app.get("/readings/:id", async (req, res) => {
  var readings = await prisma.leitura.findMany({
    take: 20,
    orderBy: {
      timestamp: "desc",
    },
    where: {
      id_sensor: parseInt(req.params.id),
    },
  });
  console.log(readings);
  res.send(readings);
});

app.get("/ss/:idSS/readings", async (req, res) => {
  var ss = await prisma.sensor.findMany({
    where: {
      id_SS: parseInt(req.params.idSS),
    },
    select: {
      id_sensor: true,
    },
  });

  var id_list = ss.map((a) => {
    return a.id_sensor;
  });
  console.log(id_list);

  var reading = await prisma.leitura.findMany({
    where: {
      id_sensor: { in: id_list },
    },
  });

  res.send(reading);
});

app.get("/disconnect/:id", async (req, res) => {
    const value = await prisma.sistema_sensor.update({
        where:{
            id_SS: parseInt(req.params.id)
        },
        data:{
            estado: "OFF"
        }
    })
})

app.get("/change_sensor_state/:id", async (req, res) => {
  const value = await prisma.sensor.findUnique({
    where: {
      id_sensor: parseInt(req.params.id),
    },
  });
  var newState;
  if (value.estado == "ON") newState = "OFF";
  else newState = "ON";
  var sensor = await prisma.sensor.update({
    where: {
      id_sensor: parseInt(req.params.id),
    },
    data: {
      estado: newState,
    },
  });

});

app.get("/ss/", async (req, res) => {
  var all_ss = await prisma.sistema_sensor.findMany({});
  var arr = [];

  for (const [key, value] of Object.entries(all_ss)) {
    arr.push(value);
  }

  res.send(arr);
});

app.post("/post_user", async (req, res) => {
  const object = req.body;
  console.log(object);

  const user = await prisma.utilizador.findFirst({
    where:{
      username: object.username
    }
  })

  if(user == null){
    const system = await prisma.utilizador.create({
      data: {
        // id_user: parseInt(object.id_user),
        username: object.username,
        password: object.password,
        tipo_utilizador: object.tipo_utilizador
      },
    });
  
    res.send("Utilizador registado com sucesso.");
  }
  else{
    res.send("Ja existe um utilizador com esse nome.");
  }
  
});

app.post("/login", async (req, res) => {
  const object = req.body;
  const user = await prisma.utilizador.findFirst({
    where: {
      username: object.username
    }
   });

  if(user == null){
    res.send("nao existe nenhum utilizador com esse nome.");
  }
  else if(user.password == object.password){
    res.send("success.");
  }
  else{
    res.send("failed.");
  }
})

// app.post("/post_new_order", async (req, res) => {
//   const object = req.body;
//   console.log(object);
//   const system = await prisma.utilizador.create({
//     data: {
//       id_user: parseInt(object.id_user),
//       username: object.username,
//       password: object.password,
//     },
//   });

//   res.send();
// });

app.post("/post_ss", async (req, res) => {
  const object = req.body;
  console.log(object);
  const system = await prisma.sistema_sensor.create({
    data: {
      id_SS: parseInt(object.id_SS),
      nome_sistema_sensor: object.nome_sistema_sensor,
      tipo: object.tipo,
      estado: object.estado,
    },
  });

  res.send();
});

app.post("/post_reading", async (req, res) => {
  const object = req.body;
  console.log(object);

  if (object.temperatura >= 0) {
    var sensor_array = await prisma.sensor.findMany({
      where: {
        id_SS: parseInt(object.id_sistema),
      },
    });
    var sensor = sensor_array.filter((a) => a.nome_sensor == "temperatura");
    sensor = sensor[0];
    console.log(sensor);
    if (sensor == null) {
      sensor = await prisma.sensor.create({
        data: {
          id_SS: parseInt(object.id_sistema),
          nome_sensor: "temperatura",
          estado: "ON",
        },
      });
    }
    if (sensor.estado == "ON") {
      const reading = await prisma.leitura.create({
        data: {
          id_sensor: parseInt(sensor.id_sensor),
          timestamp: object.timestamp,
          dados: object.temperatura,
        },
      });
    }
  }

  if (object.humidade >= 0) {
    var sensor_array = await prisma.sensor.findMany({
      where: {
        id_SS: parseInt(object.id_sistema),
      },
    });
    var sensor = sensor_array.filter((a) => a.nome_sensor == "humidade");
    sensor = sensor[0];
    console.log(sensor);
    if (sensor == null) {
      sensor = await prisma.sensor.create({
        data: {
          id_SS: parseInt(object.id_sistema),
          nome_sensor: "humidade",
          estado: "ON",
        },
      });
    }
    if (sensor.estado == "ON") {
      const reading = await prisma.leitura.create({
        data: {
          id_sensor: parseInt(sensor.id_sensor),
          timestamp: object.timestamp,
          dados: object.humidade,
        },
      });
    }
  }

  if (object.distancia >= 0) {
    var sensor_array = await prisma.sensor.findMany({
      where: {
        id_SS: parseInt(object.id_sistema),
      },
    });
    var sensor = sensor_array.filter((a) => a.nome_sensor == "distancia");
    sensor = sensor[0];
    console.log(sensor);
      if (sensor == null) {
        sensor = await prisma.sensor.create({
          data: {
            id_SS: parseInt(object.id_sistema),
            nome_sensor: "distancia",
            estado: "ON",
          },
        });
      }
      if (sensor.estado == "ON") {
      const reading = await prisma.leitura.create({
        data: {
          id_sensor: parseInt(sensor.id_sensor),
          timestamp: object.timestamp,
          dados: object.distancia,
        },
      });
    }
  }

  if (object.movimento >= 0) {
    var sensor_array = await prisma.sensor.findMany({
      where: {
        id_SS: parseInt(object.id_sistema),
      },
    });
    var sensor = sensor_array.filter((a) => a.nome_sensor == "movimento");
    sensor = sensor[0];
    console.log(sensor);
      if (sensor == null) {
        sensor = await prisma.sensor.create({
          data: {
            id_SS: parseInt(object.id_sistema),
            nome_sensor: "movimento",
            estado: "ON",
          },
        });
      }
      if (sensor.estado == "ON") {
      const reading = await prisma.leitura.create({
        data: {
          id_sensor: parseInt(sensor.id_sensor),
          timestamp: object.timestamp,
          dados: object.movimento == 0 ? "False" : "True",
        },
      });
    }
  }

  if (object.chuva >= 0) {
    var sensor_array = await prisma.sensor.findMany({
      where: {
        id_SS: parseInt(object.id_sistema),
      },
    });
    var sensor = sensor_array.filter((a) => a.nome_sensor == "chuva");
    sensor = sensor[0];
    console.log(sensor);
      if (sensor == null) {
        sensor = await prisma.sensor.create({
          data: {
            id_SS: parseInt(object.id_sistema),
            nome_sensor: "chuva",
            estado: "ON",
          },
        });
      }
      if (sensor.estado == "ON") {
      const reading = await prisma.leitura.create({
        data: {
          id_sensor: parseInt(sensor.id_sensor),
          timestamp: object.timestamp,
          dados: object.chuva == 0 ? "False" : "True",
        },
      });
    }
  }

  res.send();
});

app.listen(PORT, (err) => {
  console.log(`http://localhost:${PORT}`);
});
