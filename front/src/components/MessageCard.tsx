import { Message } from "../model/message";

interface MessageCardProps {
  message: Message;
}

export const MessageCard = ({ message }: MessageCardProps) => {
  const msg = (() => {
    if ("name" in message) {
      return message.name;
    } else if ("message" in message) {
      return message.message;
    } else if ("timestamp" in message) {
      return message.timestamp;
    }
    return null;
  })();
  return (
    <div className={["p-4", "rounded-md"].join(" ")}>
      <p className={["text-sm", "text-gray-600"].join(" ")}>
        {message.clientId}
      </p>
      <div className={["flex", "flex-row", "justify-end"].join(" ")}>
        {msg && <p>{msg}</p>}
      </div>
    </div>
  );
};
