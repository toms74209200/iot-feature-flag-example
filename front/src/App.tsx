import { useEffect, useState } from "react";
import { connectMqtt, publishMqtt, subscribeMqtt } from "./model/mqtt";
import { ToggleButton } from "./components/ToggleButton";

function App() {
  const [subscribed, setSubscribed] = useState<boolean>(false);
  const [subscribeTopic, setSubscribeTopic] = useState<string>("");
  const [sendMessage, setSendMessage] = useState<string>("");
  const [messages, setMessages] = useState<string[]>([]);

  useEffect(() => {
    return () => {
      connectMqtt(() => {});
    };
  }, []);

  return (
    <main className={["w-full", "mt-12"].join(" ")}>
      <div
        className={[
          "m-auto",
          "max-w-3xl",
          "grid",
          "gap-8",
          "grid-cols-1",
          "md:grid-cols-4",
        ].join(" ")}
      >
        <div
          className={["m-auto", "text-2xl", "font-bold", "md:col-span-4"].join(
            " "
          )}
        >
          <h1>IoT Feature Flag demo</h1>
        </div>
        <div
          className={["flex", "flex-col", "space-y-4", "md:col-span-1"].join(
            " "
          )}
        >
          <h2 className={["text-xl", "font-bold", "text-center"].join(" ")}>
            Feature toggles
          </h2>
          <label
            className={[
              "flex",
              "flex-row",
              "items-center",
              "justify-center",
              "space-x-2",
            ].join(" ")}
          >
            Greeter
            <ToggleButton onChange={() => {}} />
          </label>
        </div>
        <div
          className={["flex", "flex-col", "space-y-4", "md:col-span-3"].join(
            " "
          )}
        >
          <div
            className={["flex", "flex-row", "justify-center", "space-x-2"].join(
              " "
            )}
          >
            <input
              type="text"
              className={["border", "rounded-md", "py-1", "px-2"].join(" ")}
              placeholder="Subscribe Topic"
              onChange={(e) => {
                setSubscribeTopic(e.target.value);
              }}
              disabled={subscribed}
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
              disabled={subscribed || subscribeTopic === ""}
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
              disabled={!subscribed}
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
              disabled={!subscribed || sendMessage === ""}
            >
              Send
            </button>
          </div>
          <ul className={["col-span-3"].join(" ")}>
            {messages.map((message, index) => (
              <li key={index} className={["my-4"].join(" ")}>
                {message}
              </li>
            ))}
          </ul>
        </div>
      </div>
    </main>
  );
}

export default App;
