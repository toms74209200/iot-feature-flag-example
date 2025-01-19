## Usage

Dotenv file is required to run the project. Set the MQTT broker URL in the `.env` file as follows.

**`.env`**

```toml
VITE_MQTT_URL=ws://localhost:8083/mqtt
```

Launch frontend app.

```bash
$ npm run dev
```

## Environments

- TypeScript 5.6.2
- Vite 6.0.5
- React 19.0.0
- Tailwind CSS 3.4.17
- [MQTT.js](https://github.com/mqttjs/MQTT.js) 5.10.3
