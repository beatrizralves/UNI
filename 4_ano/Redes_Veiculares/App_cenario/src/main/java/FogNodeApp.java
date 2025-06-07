import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;

import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.CamBuilder;
import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.CellModule;
import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.CellModuleConfiguration;
import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.ReceivedAcknowledgement;
import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.ReceivedV2xMessage;
import org.eclipse.mosaic.fed.application.ambassador.simulation.perception.index.objects.TrafficLightObject;
import org.eclipse.mosaic.fed.application.app.AbstractApplication;
import org.eclipse.mosaic.fed.application.app.api.CommunicationApplication;
import org.eclipse.mosaic.fed.application.app.api.TrafficLightApplication;
import org.eclipse.mosaic.fed.application.app.api.os.RoadSideUnitOperatingSystem;
import org.eclipse.mosaic.fed.application.app.api.os.modules.CellCommunicative;
import org.eclipse.mosaic.interactions.communication.V2xMessageTransmission;
import org.eclipse.mosaic.interactions.traffic.TrafficLightStateChange;
import org.eclipse.mosaic.interactions.traffic.TrafficLightUpdates;
import org.eclipse.mosaic.lib.objects.trafficlight.TrafficLightState;
import org.eclipse.mosaic.lib.objects.v2x.GenericV2xMessage;
import org.eclipse.mosaic.lib.objects.v2x.MessageRouting;
import org.eclipse.mosaic.lib.objects.v2x.V2xMessage;
import org.eclipse.mosaic.lib.util.scheduling.Event;
import org.eclipse.mosaic.lib.objects.trafficlight.TrafficLight;
import org.eclipse.mosaic.rti.DATA;
import org.eclipse.mosaic.rti.TIME;
import org.eclipse.mosaic.lib.objects.trafficlight.TrafficLight;

public class FogNodeApp extends AbstractApplication<RoadSideUnitOperatingSystem>
        implements CommunicationApplication, CellCommunicative {
    /**
     * Interval at which messages are sent (every 2 seconds).
     */
    private final static long TIME_INTERVAL = 2 * TIME.SECOND;
    Double speed_limit = 11.0; // 11.0 m/s = 40 km/h
    ArrayList<Vehicle> vehicles;

    // celular

    @Override
    public void onStartup() {
        getLog().infoSimTime(this, "Initialize application");
        getOs().getCellModule().enable(new CellModuleConfiguration()
                .maxDownlinkBitrate(50 * DATA.MEGABIT)
                .maxUplinkBitrate(50 * DATA.MEGABIT));

        getOs().getCellModule().enable(new CellModuleConfiguration());
        getLog().infoSimTime(this, "Activated CellModule");

        this.vehicles = new ArrayList<Vehicle>();

        getOs().getEventManager().addEvent(getOs().getSimulationTime() + TIME_INTERVAL, this);
    }

    // msg: String message = 2 + "|" + vehid + "|" speed;
    private GenericV2xMessage msg_Speed_Alert(String vehID, Double speed) {
        // mensagem enviado para o rsu
        byte[] receiverName = new byte[] { 10, 2, 0, 0 };
        String msgID = "2";

        MessageRouting routing = getOs().getCellModule().createMessageRouting()
                .topoCast(receiverName);

        String message = vehID + "|" + speed;
        return new GenericV2xMessage(routing, msgID + "|" + message, (long) message.length());

    }

    // msg: String message = 3 + "|" + veh1 + "|" + v1_edge + "|" + veh2 +
    // "|" + v2_edge + "|" + trafficLight;
    private GenericV2xMessage msg_Crash(String veh1, String v1_edge, String veh2, String v2_edge, String trafficLight) {
        // mensagem enviado para o rsu
        byte[] receiverName = new byte[] { 10, 2, 0, 0 };
        String msgID = "3";

        MessageRouting routing = getOs().getCellModule().createMessageRouting()
                .topoCast(receiverName);

        String message = msgID + "|" + veh1 + "|" + v1_edge + "|" + veh2 + "|" + v2_edge + "|" + trafficLight;
        return new GenericV2xMessage(routing, message, (long) message.length());

    }

    // msg:String message = 1 + "|" + edge + "|" + count + "|" + avgSpeed + "|" +
    // avgDelay;
    private GenericV2xMessage msg_Traffic_Summary(HashMap<String, ArrayList<Vehicle>> traffic) {
        byte[] receiverName = new byte[] { 10, 2, 0, 0 };
        String msgID = "1";

        MessageRouting routing = getOs().getCellModule().createMessageRouting().topoCast(receiverName);

        StringBuilder messageBuilder = new StringBuilder();
        messageBuilder.append(msgID);

        for (String edge : traffic.keySet()) {
            ArrayList<Vehicle> vehiclesOnEdge = traffic.get(edge);
            int count = vehiclesOnEdge.size();
            double totalSpeed = 0.0;

            for (Vehicle v : vehiclesOnEdge) {
                totalSpeed += v.speed;
            }

            double avgSpeed = count == 0 ? 0 : totalSpeed / count;
            double expectedSpeed = 13.89;
            double avgDelay = Math.max(0, expectedSpeed - avgSpeed);

            messageBuilder.append("|")
                    .append(edge).append("|")
                    .append(count).append("|")
                    .append(String.format(Locale.US, "%.4f", avgSpeed)).append("|")
                    .append(String.format(Locale.US, "%.4f", avgDelay));
        }

        String message = messageBuilder.toString();

        return new GenericV2xMessage(routing, message, (long) message.length());
    }

    @Override
    public void onShutdown() {
        getLog().infoSimTime(this, "Shutdown application");

    }

    public void sample() {

    }

    @Override
    public void processEvent(Event arg0) throws Exception {
        // calculo lane e quantidade de carros

        // ----------------------
        // criar um hashmap temporario
        HashMap<String, ArrayList<Vehicle>> traffic = new HashMap<String, ArrayList<Vehicle>>();

        ArrayList<Vehicle> vehiclesCopy = new ArrayList<Vehicle>();

        for (Vehicle vehicle : vehicles) {
            // Clona o veículo para evitar problemas de concorrência
            vehiclesCopy.add(vehicle.clone());
        }

        // percorre a lista de veículos
        for (Vehicle vehicle : vehiclesCopy) {
            // se a lane não existe, criar uma nova lane e a respetiva lista de veículos
            // (vazia)
            if (!traffic.containsKey(vehicle.Id_edge)) {
                traffic.put(vehicle.Id_edge, new ArrayList<Vehicle>());
            }
            // se a lane existe, verificar se o carro já existe
            if (!traffic.get(vehicle.Id_edge).contains(vehicle)) {
                // se o carro não existe, adicionar o carro
                traffic.get(vehicle.Id_edge).add(vehicle);
            }
        }

        ArrayList<String> lanes_qttCar = new ArrayList<String>();

        for (String lane : traffic.keySet()) {

            lanes_qttCar.add(lane + "|" + traffic.get(lane).size());

        }

        getLog().infoSimTime(this, "Fog: " + lanes_qttCar.toString());

        // envia msg V2X com as lanes e a quantidade de carros para o RSU
        getOs().getCellModule().sendV2xMessage(msg_Traffic_Summary(traffic));
        getLog().infoSimTime(this, "Fog: Mensage V2X Sent (Metrics): " + msg_Traffic_Summary(traffic).getMessageType());

        // getOs().getCellModule().sendV2xMessage(msg_Lanes_QttCar(lanes_qttCar));
        // getLog().infoSimTime(this, "Fog: Mensage V2X Sent: " +
        // msg_Lanes_QttCar(lanes_qttCar).getMessageType());

        getOs().getEventManager().addEvent(getOs().getSimulationTime() + TIME_INTERVAL, this);

    }

    @Override
    public void onAcknowledgementReceived(ReceivedAcknowledgement arg0) {

    }

    @Override
    public void onCamBuilding(CamBuilder arg0) {

    }

    public void msg_process(Integer Id_msg, String Id_vehicle, String Id_edge, Double speed,
            Double acelaration, Double direction, Double lat, Double lon, String signal) {

        // Verifica se o veículo já existe
        Vehicle existingVehicle = null;
        for (Vehicle v : vehicles) {
            if (v.Id_vehicle.equals(Id_vehicle)) {
                existingVehicle = v;
                break;
            }
        }

        if (existingVehicle == null) {
            // Veículo novo
            Vehicle vehicle = new Vehicle(Id_vehicle, Id_edge, speed, acelaration, direction, lat, lon, signal);
            vehicles.add(vehicle);

            vehicle_overSpeed(vehicle); // Verifica se o veículo está acima do limite de velocidade
            // vehicle_averageSpeed(vehicle);

            vehicle_detectCollisions(vehicle); // Detecta colisões com outros veículos

            getLog().infoSimTime(this, "Fog: New vehicle added: " + Id_vehicle);

        } else {
            // Veículo existente - atualiza dados
            existingVehicle.update(Id_edge, speed, acelaration, direction, lat, lon, signal);

            vehicle_overSpeed(existingVehicle);
            // vehicle_averageSpeed(existingVehicle);

            vehicle_detectCollisions(existingVehicle); // Detecta colisões com outros veículos

            getLog().infoSimTime(this, "Fog: Vehicle updated: " + Id_vehicle);
        }

    }

    // funcao para verificar se o veículo está acima do limite de velocidade
    public void vehicle_overSpeed(Vehicle vehicle) {
        // Verifica se o veículo está acima do limite de velocidade
        if (vehicle.overSpeed(speed_limit)) {
            getLog().infoSimTime(this, "Fog: Vehicle " + vehicle.Id_vehicle + " is over the speed limit: "
                    + vehicle.speed);
            // Envia mensagem para a RSU
            getOs().getCellModule().sendV2xMessage(msg_Speed_Alert(vehicle.Id_vehicle, vehicle.speed));
        }
    }

    // função para detectar colisões entre veículos
    private void vehicle_detectCollisions(Vehicle v1) {
        double futureTime = 1; // segundos
        double timeStep = 0.01;
        double safetyDistance = 1.0; // metro

        // Conversão de latitude/longitude para coordenadas cartesianas aproximadas
        double latMeter = 111320;
        double lonMeter = 40075000 * Math.cos(Math.toRadians(v1.lat)) / 360;
        // Calcula a posição atual do veículo v1 em coordenadas cartesianas
        double x1 = v1.lon * lonMeter;
        double y1 = v1.lat * latMeter;

        for (Vehicle v2 : vehicles) {
            if (v1.equals(v2))
                continue;

            double x2 = v2.lon * lonMeter;
            double y2 = v2.lat * latMeter;

            double approxDist = Math.hypot(x1 - x2, y1 - y2);

            if (approxDist > 100)
                continue; // Ignora veículos muito distantes

            for (double t = timeStep; t <= futureTime; t += timeStep) {
                double fx1 = x1 + v1.speed * Math.cos(Math.toRadians(v1.direction)) * t;
                double fy1 = y1 + v1.speed * Math.sin(Math.toRadians(v1.direction)) * t;

                double fx2 = x2 + v2.speed * Math.cos(Math.toRadians(v2.direction)) * t;
                double fy2 = y2 + v2.speed * Math.sin(Math.toRadians(v2.direction)) * t;

                double futureDist = Math.hypot(fx1 - fx2, fy1 - fy2);

                if (futureDist <= safetyDistance) {
                    getLog().infoSimTime(this, "Fog: Crash Possibility: " + v1.Id_vehicle + " and " + v2.Id_vehicle);

                    String traffic_ID = "null";
                    if (List.of("-E2", "-E1", "-E3", "E0").contains(v1.Id_edge)) {
                        traffic_ID = "J1";
                    }

                    getOs().getCellModule().sendV2xMessage(
                            msg_Crash(v1.Id_vehicle, v1.Id_edge, v2.Id_vehicle, v2.Id_edge, traffic_ID));


                    break;
                }
            }
        }
    }

    @Override
    // recebe mensagem da RSU e separa o payload
    public void onMessageReceived(ReceivedV2xMessage receivedV2xMessage) {
        V2xMessage message = receivedV2xMessage.getMessage();
        if (message instanceof GenericV2xMessage) {
            String payload = ((GenericV2xMessage) message).getMessageType();

            getLog().infoSimTime(this, "Fog: Mensage V2X Received: " + payload);

            String[] parts = payload.split("\\|");

            this.msg_process(Integer.parseInt(parts[0]), parts[1], parts[2], Double.parseDouble(parts[3]),
                    Double.parseDouble(parts[4]), Double.parseDouble(parts[5]), Double.parseDouble(parts[6]),
                    Double.parseDouble(parts[7]), parts[8]);

        }
    }

    @Override
    public void onMessageTransmitted(V2xMessageTransmission arg0) {

    }

    @Override
    public CellModule getCellModule() {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'getCellModule'");
    }

    private class Vehicle {

        public String Id_vehicle;
        public String Id_edge;
        public Double speed;
        public Double acelaration;
        public Double direction;
        public Double lat;
        public Double lon;
        public String signal;

        public Vehicle(String Id_vehicle, String Id_edge, Double speed, Double acelaration, Double direction,
                Double lat,
                Double lon, String signal) {
            this.Id_vehicle = Id_vehicle;
            this.Id_edge = Id_edge;
            this.speed = speed;
            this.acelaration = acelaration;
            this.direction = direction;
            this.lat = lat;
            this.lon = lon;
            this.signal = signal;

        }

        private void update(String Id_edge, Double speed, Double acelaration, Double direction,
                Double lat, Double lon, String signal) {
            this.Id_edge = Id_edge;
            this.speed = speed;
            this.acelaration = acelaration;
            this.direction = direction;
            this.lat = lat;
            this.lon = lon;
            this.signal = signal;

        }

        private boolean overSpeed(Double speed_limit) {
            if (this.speed > speed_limit) {
                return true;
            } else {
                return false;
            }
        }

        private boolean compare(Vehicle vehicle) {
            if (this.Id_vehicle.equals(vehicle.Id_vehicle)) {
                return true;
            } else {
                return false;
            }
        }

        private boolean equals(Vehicle vehicle) {
            if (this.Id_vehicle.equals(vehicle.Id_vehicle)) {
                return true;
            } else {
                return false;
            }
        }

        public Vehicle clone() {
            return new Vehicle(this.Id_vehicle, this.Id_edge, this.speed, this.acelaration,
                    this.direction, this.lat, this.lon, this.signal);
        }
    }

}
