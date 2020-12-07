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


const DEBUG = true;
const domain = DEBUG ? 'http://localhost:8000' : 'http://192.168.4.1';
const LOADING_TIME =  DEBUG ? 100 : 3500;

const minutes = seconds => seconds * 60;
const hours = seconds => seconds * 60 * 60;

const REGIMES = [
    {
        title: '5 минути работа на 30 минути',
        workSeconds: minutes(5),
        restSeconds: minutes(30),
    },
    {
        title: '3 минути работа на 30 минути',
        workSeconds: minutes(3),
        restSeconds: minutes(30),
    },
    {
        title: '5 минути работа на 1 час',
        workSeconds: minutes(5),
        restSeconds: hours(1),
    },
    {
        title: '3 минути работа на 1 час',
        workSeconds: minutes(3),
        restSeconds: hours(1),
    },
    {
        title: '10 минути работа на 2 часа',
        workSeconds: minutes(10),
        restSeconds: hours(2),
    },
    {
        title: '6 минути работа на 2 часа',
        workSeconds: minutes(10),
        restSeconds: hours(2),
    },
    {
        title: 'Тестов режим (20 секунди работа, 20 секунди почивка)',
        workSeconds: minutes(10),
        restSeconds: hours(2),
    },
];


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

async function fetchWithTimeout(resource) {
  const controller = new AbortController();
  const id = setTimeout(() => controller.abort(), 3000);

  const response = await fetch(resource, {
    signal: controller.signal,
  });
  clearTimeout(id);

 return response;
}


export default class App extends React.Component {
  state = {
    isOnline: false,
    workTime: null,
    restTime: null,
    isOn: false,
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

  connect = () => fetchWithTimeout(`${domain}/config/get/`)
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
    }).catch(er => this.setState({isOnline: false, isOn: false}))

  componentDidMount() {
    setTimeout(() => this.setState({ isInitiallyLoading: false }), LOADING_TIME);
    this.heartbeatAnimation();
    setInterval(() => this.connect(), 3000);
  }

  verboseName = miliseconds => {
      const seconds = miliseconds / 1000;
      if (seconds < 60)
          return `${seconds} секунди`

      return `${(seconds - seconds % 60) / 60} минути и ${Math.round(seconds % 60, 2)} секунди`
  }
  timeLeftVerbose =  () => {
      if (!this.state.timeLeft) {
          return ''
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
              <Text style={{color: 'white', marginTop: 10}}>Време работа: {this.state.workTime ? this.verboseName(this.state.workTime) : 'N/A'}</Text>
              <Text style={{color: 'white', marginTop: 10}}>Време почивка: {this.state.restTime ? this.verboseName(this.state.restTime) : 'N/A'}</Text>
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
          <Text style={{ color: "#fff", textAlign: "center", marginTop: 10 }}>Включена</Text>
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
          <Text style={{ color: "#fff", textAlign: "center", marginTop: 10 }}>Изключена</Text>
        </View>
      );
  };

    errorMessage = () => (
        <View style={{background: '#ff3333', width: '80vw', padding: 10, marginTop: 10}}>
            <Text style={{fontSize: '20px', color: 'white', textAlign: 'center'}}>Грешка: Нямате WIFI връзка с бактерицидната лампа!!</Text>
            <Text style={{color: 'white', textAlign: 'left', fontSize: '15px', marginTop: '10px'}}>1. Проверете захранването на бактерицидната лампа</Text>
            <Text style={{color: 'white', textAlign: 'left', fontSize: '15px', marginTop: '10px'}}>2. Проверете режима на бактерицидната лампа</Text>
            <Text style={{color: 'white', textAlign: 'left', fontSize: '15px', marginTop: '10px'}}>3. Свърженете мобилното си устройство към WIFI с име "UVLampCentralBalkan"</Text>
        
        </View>
    )

    getButton = (title, workSeconds, restSeconds) => (
        <View style={styles.buttonBox}>
            <Button
                title={title}
                color={buttonColor}
                onPress={() => setConfig(workSeconds, restSeconds)}
            />
        </View>
    )

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
        {this.state.isOnline ? '' : this.errorMessage()}

        <Text style={styles.subTitle}>Режими на работа</Text>

        {REGIMES.map(({title, workSeconds, restSeconds}) => this.getButton(title, workSeconds, restSeconds))}
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
