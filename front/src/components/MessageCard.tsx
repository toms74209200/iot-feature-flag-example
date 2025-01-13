import { Message } from "../model/message";

interface MessageCardProps {
  message: Message;
}

export const MessageCard = ({ message }: MessageCardProps) => {
  return (
    <div className={["p-4", "rounded-md"].join(" ")}>
      <p className={["text-sm", "text-gray-600"].join(" ")}>
        {message.clientId}
      </p>
      <div className={["flex", "flex-row", "justify-end"].join(" ")}>
        {"name" in message ? (
          <p>{message.name}</p>
        ) : "message" in message ? (
          <p>{message.message}</p>
        ) : null}
      </div>
    </div>
  );
};
