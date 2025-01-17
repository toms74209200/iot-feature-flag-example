import { useEffect, useRef, useState } from "react";
import {
  connectMqtt,
  publishMqtt,
  subscribeMqtt,
  unsubscribeMqtt,
} from "./model/mqtt";
import { ToggleButton } from "./components/ToggleButton";
import {
  jsonToMessage,
  Message,
  messageToJson,
  TimestampFormat,
} from "./model/message";
import { MessageCard } from "./components/MessageCard";

const featureTopic = "feature";
const greeterTopic = "greeter";
const timestampTopic = "timestamp";

const clientId = crypto.randomUUID().replace(/-/g, "");

function App() {
  const [greeterSubscribed, setGreeterSubscribed] = useState<boolean>(false);
  const [timestampSubscribed, setTimestampSubscribed] =
    useState<boolean>(false);
  const [name, setName] = useState<string>("");
  const [greetings, setGreetings] = useState<Message[]>([]);
  const [timestamps, setTimestamps] = useState<Message[]>([]);
  const [greeterEnabled, setGreeterEnabled] = useState<boolean>(false);
  const [timestampFormat, setTimestampFormat] =
    useState<TimestampFormat>("iso8601");

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
          if (messageJson.timestamp) {
            setTimestampFormat(messageJson.timestamp);
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
                    feature: { greeter: checked, timestamp: timestampFormat },
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
              {greeterSubscribed ? "Subscribed" : "Subscribe"}
              <ToggleButton
                onChange={(checked) => {
                  if (checked) {
                    subscribeMqtt(greeterTopic, (_topic, message) => {
                      const msg = jsonToMessage(message);
                      if ("name" in msg || "message" in msg) {
                        setGreetings((prev) => [msg, ...prev]);
                      }
                    });
                  } else {
                    unsubscribeMqtt(greeterTopic);
                    setGreetings([]);
                  }
                  setGreeterSubscribed(checked);
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
              disabled={!greeterSubscribed}
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
              disabled={!greeterSubscribed || name === ""}
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
        <div
          className={["m-auto", "text-2xl", "font-bold", "md:col-span-4"].join(
            " "
          )}
        >
          <h2 className={["text-xl", "font-bold", "text-center"].join(" ")}>
            Timestamp
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
          {[
            {
              name: "ISO 8601",
              value: "iso8601" as TimestampFormat,
            },
            {
              name: "Unix time",
              value: "epoch" as TimestampFormat,
            },
          ].map((timestamp) => (
            <label
              key={timestamp.value}
              className={["grid", "grid-cols-2", "items-center"].join(" ")}
            >
              <input
                type="radio"
                name="timestamp"
                value={timestamp.value}
                checked={timestampFormat === timestamp.value}
                onChange={(e) => {
                  setTimestampFormat(e.target.value as TimestampFormat);
                  publishMqtt(
                    featureTopic,
                    messageToJson({
                      clientId: clientId,
                      feature: {
                        greeter: greeterEnabled,
                        timestamp: e.target.value as TimestampFormat,
                      },
                    })
                  );
                }}
              />
              {timestamp.name}
            </label>
          ))}
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
              {timestampSubscribed ? "Subscribed" : "Subscribe"}
              <ToggleButton
                onChange={(checked) => {
                  if (checked) {
                    subscribeMqtt(timestampTopic, (_topic, message) => {
                      const msg = jsonToMessage(message);
                      if ("timestamp" in msg) {
                        setTimestamps((prev) => [msg, ...prev]);
                      }
                    });
                  } else {
                    unsubscribeMqtt(timestampTopic);
                    setTimestamps([]);
                  }
                  setTimestampSubscribed(checked);
                }}
              />
            </label>
          </div>
          <div
            className={["flex", "flex-row", "justify-center", "space-x-2"].join(
              " "
            )}
          >
            <button
              className={[
                "bg-teal-600",
                "text-white",
                "font-bold",
                "rounded-md",
                "py-1",
                "px-3",
                "active:bg-teal-700/90",
                "disabled:opacity-25",
              ].join(" ")}
              onClick={() => {
                publishMqtt(
                  timestampTopic,
                  messageToJson({ clientId: clientId })
                );
                setName("");
              }}
              disabled={!timestampSubscribed}
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
            {timestamps.map((timestamp, index) => (
              <li
                key={index}
                className={[
                  "max-w-80",
                  "mx-auto",
                  timestamp.clientId === clientId
                    ? "translate-x-2"
                    : "-translate-x-2",
                  timestamp.clientId === clientId
                    ? "bg-amber-50"
                    : "bg-gray-50",
                ].join(" ")}
              >
                <MessageCard message={timestamp} />
              </li>
            ))}
          </ul>
        </div>
      </div>
    </main>
  );
}

export default App;
