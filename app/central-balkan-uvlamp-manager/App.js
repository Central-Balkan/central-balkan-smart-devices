import { StatusBar } from "expo-status-bar";
import React from "react";
import {
  Animated,
  StyleSheet,
  Text,
  View,
  Button,
  Image,
  ActivityIndicator,
} from "react-native";
import * as axios from 'axios';



// const domain = 'http://192.168.4.1';
const domain = 'http://localhost:8000';
// const domain = 'http://0.0.0.0:8000';

const setConfig = (work, rest) => {
  console.log(work, rest);
  console.log(JSON.stringify({ work, rest }));
  fetch(`${domain}/config/set/`, {
    method: "POST",
    mode: "no-cors",
    headers: {
      "Content-Type": "application/x-www-form-urlencoded",
    },
    body: `work=${work}&rest=${rest}`,
  });
};

const bulbSize = 150;
const buttonColor = "#3f5ea2";

async function fetchWithTimeout(resource, options) {
  const { timeout = 5000 } = options;

  const controller = new AbortController();
  const id = setTimeout(() => controller.abort(), timeout);

  const response = await fetch(resource, {
    ...options,
    signal: controller.signal,
  });
  clearTimeout(id);

 return response;
}


export default class App extends React.Component {
  state = {
    isOnline: true,
    workTime: null,
    restTime: null,
    isOn: true,
    timeLeft: 0,

    isInitiallyLoading: true,
    statusBosAnimatedOpacity: new Animated.Value(1),
  };

  heartbeatAnimation = () => {
    // Will change fadeAnim value to 1 in 5 seconds
    Animated.timing(this.state.statusBosAnimatedOpacity, {
      toValue: 0.2,
      duration: 2000,
    }).start();
  };

  connect = () => {
  return fetch(`${domain}/config/get/`)
    .then((response) => response.json())
    .then((json) => {
        console.log('asd')
        console.log(json)
            this.setState({
              isOnline: true,
              isOn: true,
              workTime: json.workTime,
              restTime: json.restTime,
              timeLeft: json.timeLeft,
            });
    })
  }

  componentDidMount() {
    setTimeout(() => this.setState({ isInitiallyLoading: false }), 1000);
    this.heartbeatAnimation();
    setInterval(() => this.connect(), 3000);
  }

  verboseName = miliseconds => {
      const seconds = miliseconds / 1000;
      if (seconds < 60)
          return `${seconds} секунди`

      return `${(seconds - seconds % 60) / 60} минути и ${seconds % 60} секунди`
  }
  timeLeftVerbose =  () => {
      if (!this.state.timeLeft) {
          return 'N/A'
      }

      if (this.state.isOn) {
          return  'Изключване след ' + this.verboseName(this.state.timeLeft);
      } else {
          return  'Включване след ' + this.verboseName(this.state.timeLeft);
      }
  }

  currentProgram = () => {
      return (
          <View style={{color: 'white'}}>
              <Text style={{color: 'white'}}>Време работа: {this.state.workTime ? this.verboseName(this.state.workTime) : 'N/A'}</Text>
              <Text style={{color: 'white'}}>Време почивка: {this.state.restTime ? this.verboseName(this.state.restTime) : 'N/A'}</Text>
              <Text style={{color: 'white'}}>{this.timeLeftVerbose()}</Text>
          </View>
      )

  }
  currentState = () => {
    if (this.state.isOn)
      return (
        <View>
          <Image
            style={{
              background: "#fff",
              width: bulbSize,
              height: bulbSize,
              marginTop: 20,
            }}
            source={require("./assets/workingLamp.jpg")}
          />
          <Text style={{ color: "#fff", textAlign: "center" }}>Включена</Text>
        </View>
      );
    else
      return (
        <View>
          <Image
            style={{
              background: "#fff",
              width: bulbSize,
              height: bulbSize,
              marginTop: 20,
            }}
            source={require("./assets/bulb-off.svg")}
          />
          <Text style={{ color: "#fff", textAlign: "center" }}>Изключена</Text>
        </View>
      );
  };
  render() {
    if (this.state.isInitiallyLoading) {
      return (
        <View style={styles.container}>
          <Text style={styles.title}>Централен Балкан ЕООД</Text>
          <Text style={styles.title}>Бактерицидна UV лампа</Text>
          <ActivityIndicator size="large" />
        </View>
      );
    }

    return (
      <View style={styles.container}>
        <Text style={styles.title}>Централен Балкан ЕООД</Text>
        <Text style={styles.title}>Бактерицидна UV лампа</Text>
        <View>
            <Text style={styles.statusBoxLabel}>
                Статус на връзката: {this.state.isOn ? "Има връзка с устройството" : "Няма връзка с устройството"}
            </Text>
          <Animated.View
            style={{
              backgroundColor: this.state.isOnline ? "#3aff3a" : 'red',
              opacity: this.state.statusBosAnimatedOpacity,
              width: 20,
              height: 20,
              borderRadius: "50%",
              border: "1px solid white",
            }}
          ></Animated.View>
        </View>
        {this.currentState()}
        {this.currentProgram()}

        <Text style={styles.subTitle}>Режими на работа</Text>

        <View style={styles.buttonBox}>
          <Button
              title="
              Търговска зала - интезивен 
              (5 минути работа, 2 часа почивка)
              "
            color={buttonColor}
            onPress={() => setConfig(5 * 60, 2 * 60 * 60)}
          />
        </View>

        <View style={styles.buttonBox}>
          <Button
              title="
              Офис - интезивен 
              (3 минути работа, 2 часа почивка)
              "
            color={buttonColor}
            onPress={() => setConfig(3 * 60, 2 * 60 * 60)}
          />
        </View>

        <View style={styles.buttonBox}>
          <Button
            title="10 минути работа на 4 часа"
            color={buttonColor}
            onPress={() => setConfig(10 * 60, 4 * 60 * 60)}
          />
        </View>

        <View style={styles.buttonBox}>
          <Button
            title="6 минути работа на 4 часа"
            color={buttonColor}
            onPress={() => setConfig(6 * 60, 4 * 60 * 60)}
          />
        </View>

        <View style={styles.buttonBox}>
          <Button
            title="15 минути работа на 6 часа"
            color={buttonColor}
            onPress={() => setConfig(15 * 60, 6 * 60 * 60)}
          />
        </View>

        <View style={styles.buttonBox}>
          <Button
            title="9 минути работа на 6 часа"
            color={buttonColor}
            onPress={() => setConfig(9 * 60, 6 * 60 * 60)}
          />
        </View>

        <View style={styles.buttonBox}>
          <Button
            title="30 секунди работа / 30 секунди почивка (тест)"
            color={buttonColor}
            onPress={() => setConfig(30, 30)}
          />
        </View>

        <StatusBar style="auto" />
      </View>
    );
  }
}

const styles = StyleSheet.create({
  buttonBox: {
    marginTop: 10,
    width: "90vw",
  },
  title: {
    color: "#ffffff",
    fontSize: "24px",
    marginTop: 10,
    padding: 10,
  },
  subTitle: {
    color: "#ffffff",
    fontSize: "20px",
    marginTop: 30,
  },
  container: {
    flex: 1,
    backgroundColor: "#0f243e",
    alignItems: "center",
  },
  statusBoxLabel: {
    color: "white",
  },
});
