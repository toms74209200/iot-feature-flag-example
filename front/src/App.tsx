import { useEffect, useRef, useState } from "react";
import {
  connectMqtt,
  publishMqtt,
  subscribeMqtt,
  unsubscribeMqtt,
} from "./model/mqtt";
import { ToggleButton } from "./components/ToggleButton";
import { jsonToMessage, Message, messageToJson } from "./model/message";
import { MessageCard } from "./components/MessageCard";

const featureTopic = "feature";
const greeterTopic = "greeter";

const clientId = crypto.randomUUID().replace(/-/g, "");

function App() {
  const [subscribed, setSubscribed] = useState<boolean>(false);
  const [name, setName] = useState<string>("");
  const [greetings, setGreetings] = useState<Message[]>([]);
  const [greeterEnabled, setGreeterEnabled] = useState<boolean>(false);

  const greeterEnabledRef = useRef<HTMLInputElement>(null);

  useEffect(() => {
    return () => {
      connectMqtt(() => {
        subscribeMqtt(featureTopic, (_topic, message) => {
          const messageJson = JSON.parse(message);
          if (messageJson.client_id === clientId) {
            return;
          }
          if (messageJson.feature) {
            setGreeterEnabled(messageJson.feature.greeter);
          }
        });
      });
    };
  }, []);

  useEffect(() => {
    if (greeterEnabledRef.current) {
      greeterEnabledRef.current.checked = greeterEnabled;
    }
  }, [greeterEnabled]);

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
          className={["m-auto", "text-2xl", "font-bold", "md:col-span-4"].join(
            " "
          )}
        >
          <h2 className={["text-xl", "font-bold", "text-center"].join(" ")}>
            Greeter
          </h2>
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
            <ToggleButton
              ref={greeterEnabledRef}
              initialChecked={greeterEnabled}
              onChange={(checked) => {
                publishMqtt(
                  featureTopic,
                  messageToJson({
                    clientId: clientId,
                    feature: { greeter: checked },
                  })
                );
                setGreeterEnabled(checked);
              }}
            />
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
            <label
              className={[
                "flex",
                "flex-row",
                "items-center",
                "justify-center",
                "space-x-2",
              ].join(" ")}
            >
              {subscribed ? "Subscribed" : "Subscribe"}
              <ToggleButton
                onChange={(checked) => {
                  if (checked) {
                    subscribeMqtt(greeterTopic, (_topic, message) => {
                      console.log(message);
                      const msg = jsonToMessage(message);
                      console.dir(msg);
                      if ("name" in msg || "message" in msg) {
                        setGreetings((prev) => [msg, ...prev]);
                      }
                    });
                  } else {
                    unsubscribeMqtt(greeterTopic);
                    setGreetings([]);
                  }
                  setSubscribed(checked);
                }}
              />
            </label>
          </div>
          <div
            className={["flex", "flex-row", "justify-center", "space-x-2"].join(
              " "
            )}
          >
            <input
              type="text"
              className={["border", "rounded-md", "py-1", "px-2"].join(" ")}
              placeholder="Send Your Name"
              value={name}
              onChange={(e) => setName(e.target.value)}
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
                publishMqtt(
                  greeterTopic,
                  messageToJson({ clientId: clientId, name: name })
                );
                setName("");
              }}
              disabled={!subscribed || name === ""}
            >
              Send
            </button>
          </div>
          <ul
            className={[
              "col-span-3",
              "h-64",
              "overflow-y-scroll",
              "[&::-webkit-scrollbar]:hidden",
              "space-y-4",
            ].join(" ")}
            style={{ scrollbarWidth: "none" }}
          >
            {greetings.map((greeting, index) => (
              <li
                key={index}
                className={[
                  "max-w-80",
                  "mx-auto",
                  greeting.clientId === clientId
                    ? "translate-x-2"
                    : "-translate-x-2",
                  greeting.clientId === clientId ? "bg-amber-50" : "bg-gray-50",
                ].join(" ")}
              >
                <MessageCard message={greeting} />
              </li>
            ))}
          </ul>
        </div>
      </div>
    </main>
  );
}

export default App;
