import { StatusBar } from 'expo-status-bar';
import React from 'react';
import { StyleSheet, Text, View, Button, Image } from 'react-native';

const setConfig = (work, rest) => {
    console.log(work, rest)
    console.log(JSON.stringify({work, rest}))
    fetch('http://192.168.4.1/config/set/', {
        method: 'POST',
        mode: 'no-cors',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded'
        },
        body: `work=${work}&rest=${rest}`
    });
}

const bulbSize = 150;
const buttonColor = '#3f5ea2';

export default function App() {
  return (
    <View style={styles.container}>
      <Text style={styles.title}>Централен Балкан ЕООД</Text>
      <Text style={styles.title}>Бактерицидна UV лампа</Text>
      <Image style={{background: '#fff', width: bulbSize, height: bulbSize, marginTop: 20}}source={require('./assets/bulb-on.svg')}/>
      <Text style={{color: '#fff'}}>Включена</Text>
      <Image style={{background: '#fff', width: bulbSize, height: bulbSize, marginTop: 20}}source={require('./assets/bulb-off.svg')}/>
      <Text style={{color: '#fff'}}>Изключена</Text>

      <Text style={styles.subTitle}>Режими на работа</Text>

      <View style={styles.buttonBox}>
          <Button
              title='5 минути работа на 2 часа'
              color={buttonColor}
              onPress={() => setConfig(5 * 60, 2 * 60 * 60)}
          />
      </View>

      <View style={styles.buttonBox}>
          <Button
              title='3 минути работа на 2 часа'
              color={buttonColor}
              onPress={() => setConfig(3 * 60, 2 * 60 * 60)}
          />
      </View>

      <View style={styles.buttonBox}>
          <Button
              title='10 минути работа на 4 часа'
              color={buttonColor}
              onPress={() => setConfig(10 * 60, 4 * 60 * 60)}
          />
      </View>

      <View style={styles.buttonBox}>
          <Button
              title='6 минути работа на 4 часа'
              color={buttonColor}
              onPress={() => setConfig(6 * 60, 4 * 60 * 60)}
          />
      </View>

      <View style={styles.buttonBox}>
          <Button
              title='15 минути работа на 6 часа'
              color={buttonColor}
              onPress={() => setConfig(15 * 60, 6 * 60 * 60)}
          />
      </View>

      <View style={styles.buttonBox}>
          <Button
              title='9 минути работа на 6 часа'
              color={buttonColor}
              onPress={() => setConfig(9 * 60, 6 * 60 * 60)}
          />
      </View>

      <View style={styles.buttonBox}>
          <Button
              title='30 секунди работа / 30 секунди почивка (тест)'
              color={buttonColor}
              onPress={() => setConfig(30, 30)}
          />

      </View>

      <StatusBar style="auto" />
    </View>
  );
}

const styles = StyleSheet.create({
  buttonBox: {
      marginTop: 10,
      width: '90vw',
  },
  title: {
    color: '#ffffff',
    fontSize: '24px',
    marginTop: 10,
      padding: 10,
  },
  subTitle: {
    color: '#ffffff',
    fontSize: '20px',
    marginTop: 30,
  },
  container: {
    flex: 1,
    backgroundColor: '#0f243e',
    alignItems: 'center',
  },
});
