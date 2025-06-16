DROP DATABASE IF EXISTS pit_db;
CREATE DATABASE pit_db;
USE pit_db;

CREATE TABLE utilizador (
  id_user INT NOT NULL AUTO_INCREMENT,
  tipo_utilizador INT,
  username VARCHAR(100) NOT NULL,
  password VARCHAR(100) NOT NULL,
  PRIMARY KEY (id_user)
);



CREATE TABLE sistema_sensor (
  id_SS INT NOT NULL,
  id_user INT NOT NULL,
  nome_sistema_sensor VARCHAR(100) NOT NULL,
  tipo VARCHAR(100) NOT NULL,
  estado VARCHAR(100) NOT NULL,
  PRIMARY KEY (id_SS),
  FOREIGN KEY (id_user) REFERENCES utilizador(id_user)
);

CREATE TABLE sensor (
  id_sensor INT NOT NULL AUTO_INCREMENT,
  id_SS INT NOT NULL,
  nome_sensor VARCHAR(100) NOT NULL,
  estado VARCHAR(100) NOT NULL,
  PRIMARY KEY (id_sensor),
  FOREIGN KEY (id_SS) REFERENCES sistema_sensor(id_SS)
);

CREATE TABLE leitura (
  id_leitura INT NOT NULL AUTO_INCREMENT,
  id_sensor INT NOT NULL,
  timestamp VARCHAR(200) NOT NULL,
  dados VARCHAR(100) NOT NULL,
  PRIMARY KEY (id_leitura),
  FOREIGN KEY (id_sensor) REFERENCES sensor(id_sensor)
);


ALTER TABLE sensor AUTO_INCREMENT = 0;
ALTER TABLE leitura AUTO_INCREMENT = 0;
ALTER TABLE utilizador AUTO_INCREMENT = 0;