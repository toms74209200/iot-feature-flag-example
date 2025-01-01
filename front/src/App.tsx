import { useState } from "react";
import { connectMqtt, publishMqtt, subscribeMqtt } from "./model/mqtt";

function App() {
  const [connected, setConnected] = useState<boolean>(false);
  const [subscribed, setSubscribed] = useState<boolean>(false);
  const [subscribeTopic, setSubscribeTopic] = useState<string>("");
  const [sendMessage, setSendMessage] = useState<string>("");
  const [messages, setMessages] = useState<string[]>([]);

  return (
    <main className={["w-full", "mt-12"].join(" ")}>
      <div
        className={[
          "m-auto",
          "max-w-3xl",
          "flex",
          "flex-col",
          "space-y-8",
        ].join(" ")}
      >
        <div
          className={["flex", "flex-row", "justify-center", "space-x-2"].join(
            " "
          )}
        >
          <button
            className={[
              "bg-blue-600",
              "text-white",
              "font-bold",
              "rounded-md",
              "px-3",
              "active:bg-blue-700/90",
              "disabled:opacity-25",
            ].join(" ")}
            onClick={() => {
              connectMqtt(() => {});
              setConnected(true);
            }}
            disabled={connected}
          >
            {connected ? "Connected" : "Connect"}
          </button>
          <input
            type="text"
            className={["border", "rounded-md", "py-1", "px-2"].join(" ")}
            placeholder="Subscribe Topic"
            onChange={(e) => {
              setSubscribeTopic(e.target.value);
            }}
            disabled={!connected || subscribed}
          />
          <button
            className={[
              "bg-amber-600",
              "text-white",
              "font-bold",
              "rounded-md",
              "px-3",
              "active:bg-amber-700/90",
              "disabled:opacity-25",
            ].join(" ")}
            onClick={() => {
              subscribeMqtt(subscribeTopic, (_topic, message) => {
                setMessages((prev) => [...prev, message]);
              });
              setSubscribed(true);
            }}
            disabled={!connected || subscribed || subscribeTopic === ""}
          >
            {subscribed ? "Subscribed" : "Subscribe"}
          </button>
        </div>
        <div
          className={["flex", "flex-row", "justify-center", "space-x-2"].join(
            " "
          )}
        >
          <input
            type="text"
            className={["border", "rounded-md", "py-1", "px-2"].join(" ")}
            placeholder="Send Message"
            value={sendMessage}
            onChange={(e) => setSendMessage(e.target.value)}
            disabled={!connected || !subscribed}
          />
          <button
            className={[
              "bg-teal-600",
              "text-white",
              "font-bold",
              "rounded-md",
              "px-3",
              "active:bg-teal-700/90",
              "disabled:opacity-25",
            ].join(" ")}
            onClick={() => {
              publishMqtt("test", sendMessage);
              setSendMessage("");
            }}
            disabled={!connected || !subscribed || sendMessage === ""}
          >
            Send
          </button>
        </div>
        <ul>
          {messages.map((message, index) => (
            <li key={index} className={["my-4"].join(" ")}>
              {message}
            </li>
          ))}
        </ul>
      </div>
    </main>
  );
}

export default App;
