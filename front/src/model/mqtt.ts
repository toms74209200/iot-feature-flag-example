import mqtt from "mqtt";

const url = import.meta.env.VITE_MQTT_URL || `ws://localhost:8083/mqtt`;

const mqtt_client = mqtt.connect(url);

export const connectMqtt = (onConnect: () => void) => {
  mqtt_client.on("connect", onConnect);
};

export const subscribeMqtt = (
  topic: string,
  onMessage: (topic: string, message: string) => void
) => {
  mqtt_client.subscribe(topic);
  mqtt_client.on("message", (topic, message) => {
    onMessage(topic, message.toString());
  });
};

export const unsubscribeMqtt = (topic: string) => {
  mqtt_client.unsubscribe(topic);
};

export const publishMqtt = (topic: string, message: string) => {
  mqtt_client.publish(topic, message);
};
