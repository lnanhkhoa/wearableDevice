 import React, { Component } from 'react'
 import {Platform, View, Text, StyleSheet} from 'react-native'

import { Router, Scene, Tabs, Stack } from 'react-native-router-flux'
import { connect } from 'react-redux'

import HomeComponent from './HomeComponent'
import ProfileComponent from './ProfileComponent'
import RunningComponent from './RunningComponent'
import SettingsComponent from './SettingsComponent'

import DeviceConnectionComponent from './DeviceConnectionComponent'
import ScannedDevicesComponent from '../scanning/ScannedDevicesComponent'
import ADeviceComponent from './ADeviceComponent'

import ServicesComponent from '../services/ServicesComponent'
import CharacteristicsComponent from '../characteristics/CharacteristicsComponent'
import CharacteristicDetailsComponent from '../characteristics/CharacteristicDetailsComponent'

// import TabView from './TabView'

import * as SceneConst from '../scene/Const'
import { Actions } from 'react-native-router-flux'
import * as ble from '../ble/BleActions'

const RouterWithRedux = connect()(Router)

const TabIcon = ({ selected, title }) => {
  return (
    <Text style={{color: selected ? 'red' :'black'}}>{title}</Text>
  );
}

class RootComponent extends Component {
	state={

  }

  _onBackScannedDevice=()=>{
    this.props.stopScan();
    Actions.pop();
  }

  render() {
 		return (
 			<RouterWithRedux>
      <Scene key="root"> 
        <Scene tabs key="tabbar" tabBarStyle={styles.tabBarStyle} 
          activeBackgroundColor="white" inactiveBackgroundColor="rgba(255, 0, 0, 0.5)" 
			  >
          <Scene key={SceneConst.HOME_SCENE} title="Infomation" icon={TabIcon}>
            <Scene key='info'  component={HomeComponent} title="Information" />
          </Scene>
          <Scene key={SceneConst.PROFILE_SCENE} title="Profile" icon={TabIcon}>
            <Scene key="profile" component={ProfileComponent} title="Profile" />
          </Scene>
          <Scene key={SceneConst.SERVICES_SCENE} title="Running" icon={TabIcon}>
            <Scene key="profile" component={ServicesComponent} title="Running"  />
          </Scene>
          <Scene key={SceneConst.SETTINGS_SCENE} title="Setting" icon={TabIcon}>
            <Scene key="settings1" component={SettingsComponent} title="Settings"  />
          </Scene>
        </Scene>
          <Scene key={SceneConst.DEVICES_SCENE} title="Device Connection" component={ScannedDevicesComponent} onBack={this._onBackScannedDevice} />
          <Scene key={SceneConst.ADEVICE_SCENE} title="A Device" component={ADeviceComponent} />
          <Scene key={SceneConst.CHARACTERISTICS_SCENE} component={CharacteristicsComponent} title="Running" />
          <Scene key={SceneConst.CHARACTERISTIC_DETAILS_SCENE} component={CharacteristicDetailsComponent} title="Running" />
      </Scene>
    </RouterWithRedux>
 		);
 	}
}

const mapStateToProps = (state) => ({
  selectedDeviceId: state.getIn(['ble', 'selectedDeviceId']),
  sceneState: state.getIn(['route', 'state'])
});

const mapDispatchToProps = {
  stopScan: ble.stopScan,
	changeDeviceState: ble.changeDeviceState
}


const styles = StyleSheet.create({
  container: {
    flex: 1, backgroundColor: 'transparent', justifyContent: 'center',
    alignItems: 'center',
  },
  tabBarStyle: {
    backgroundColor: '#eee',
  },
  tabBarSelectedItemStyle: {
    backgroundColor: '#ddd',
  },
});

export default connect(
	mapStateToProps, 
	mapDispatchToProps
)(RootComponent)