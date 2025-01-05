type Message = {
  clientId: string;
} & (GreeterMessage | GreeterReplyMessage | FeatureMessage);

type GreeterMessage = {
  name: string;
};

type GreeterReplyMessage = {
  message: string;
};

type FeatureMessage = {
  feature: {
    greeter: boolean;
  };
};

const messageToJson = (message: Message): string => {
  const snakeCaseMessage = Object.entries(message).reduce((acc, e) => {
    const snakeCaseKey = e[0].replace(
      /([A-Z])/g,
      (match) => `_${match.toLowerCase()}`
    );
    return { ...acc, [snakeCaseKey]: e[1] };
  }, {});

  return JSON.stringify(snakeCaseMessage);
};
