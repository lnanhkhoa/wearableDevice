'use strict';

import React, { Component } from 'react';
import { View, AppRegistry } from 'react-native';

import { applyMiddleware, createStore } from 'redux';
import { Provider } from 'react-redux';
import { createLogger } from 'redux-logger';

import ErrorComponent from './app/root/ErrorComponent'
import BleComponent from './app/ble/BleComponent'
import MainComponent from './app/root/MainComponent';
import RootComponent from './app/navigation/RootComponent'

import configureStore from './app/store/configureStore'
import { Iterable } from 'immutable';

import { checkPermission, requestPermission } from 'react-native-android-permissions';

const stateTransformer = (state) => {
  if (Iterable.isIterable(state)) {
    return state.toJS()
  } else {
    return state;
  }
};

const logger = createLogger({ stateTransformer });
const store = configureStore()

export default class App extends Component {

  componentDidMount() {
    this.checkAndGrantPermissions();
  }

  checkAndGrantPermissions() {
    checkPermission("android.permission.ACCESS_COARSE_LOCATION").then((result) => {
      console.log("Already Granted!");
      console.log(result);
    }, (result) => {
      console.log("Not Granted!");
      console.log(result);
      requestPermission("android.permission.ACCESS_COARSE_LOCATION").then((result) => {
        console.log("Granted!", result);
      }, (result) => {
        console.log("Not Granted!");
        console.log(result);
      });
    });
  }

  render() {
    return (
      <Provider store={store}>
        <View style={{flex:1}}>
          <ErrorComponent />
          <BleComponent />
          <RootComponent />
        </View>
      </Provider>
    );
  }
}
