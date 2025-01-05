export type Message = {
  clientId: string;
} & (GreeterMessage | GreeterReplyMessage | FeatureMessage);

export type GreeterMessage = {
  name: string;
};

export type GreeterReplyMessage = {
  message: string;
};

type FeatureMessage = {
  feature: {
    greeter: boolean;
  };
};

export const messageToJson = (message: Message): string => {
  const snakeCaseMessage = Object.entries(message).reduce((acc, e) => {
    const snakeCaseKey = e[0].replace(
      /([A-Z])/g,
      (match) => `_${match.toLowerCase()}`
    );
    return { ...acc, [snakeCaseKey]: e[1] };
  }, {});

  return JSON.stringify(snakeCaseMessage);
};

export const jsonToMessage = (json: string): Message => {
  return Object.entries(JSON.parse(json)).reduce((acc, e) => {
    const camelCaseKey = e[0].replace(/_([a-z])/g, (match) =>
      match[1].toUpperCase()
    );
    return { ...acc, [camelCaseKey]: e[1] };
  }, {}) as Message;
};
