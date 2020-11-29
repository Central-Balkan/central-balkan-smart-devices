import { StatusBar } from 'expo-status-bar';
import React from 'react';
import { StyleSheet, Text, View, Button } from 'react-native';

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

export default function App() {
  return (
    <View style={styles.container}>
      <Text style={styles.title}>Централен Балкан ЕООД</Text>
      <View style={styles.regimeButtons}>
          <Button
              title='10 сек. работа / 10 сек. почивка'
              color='#0f240a'
              onPress={() => setConfig(10, 10)}
          />

          <Button
              title='1 мин. работа / 1 мин. почивка'
              color='#0f240a'
              onPress={() => setConfig(60, 60)}
          />

          <Button
              title='10 мин. работа / 10 мин. почивка'
              color='#0f240a'
              onPress={() => setConfig(600, 600)}
          />
      </View>
      <StatusBar style="auto" />
    </View>
  );
}

const styles = StyleSheet.create({
  regimeButtons: {
      marginVertical: 30,
  },
  title: {
    color: '#ffffff',
    fontSize: '24px',
    marginTop: '30px'
  },
  container: {
    flex: 1,
    backgroundColor: '#0f243e',
    alignItems: 'center',
  },
});
