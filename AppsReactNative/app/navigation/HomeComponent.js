 // HomeComponent.js
 
import React, { Component } from 'react'
import { View, Text, StyleSheet } from 'react-native'
import Style from '../view/Style'

import * as ble from '../ble/BleActions'
import { connect } from 'react-redux'
import * as SceneConst from '../scene/Const'
import { Actions } from 'react-native-router-flux'
import Utils from '../ble/Utils'


class HomeComponent extends Component {
  state = {
    lastSteps: 0,
    lastHeartRate: 0,
  }

  componentWillReceiveProps(newProps) {
    if(newProps.blestate === 'CONNECTED'){
      Utils.devInfoServices(newProps);
    }
  }

  render() {
    return (
      <View style={Style.component}>
        <View style={{flexDirection:'row'}}>
          <View style={{flex:1,}}>
            <Text style={{textAlign : 'center'}}>Số bước chân gần nhất</Text>
            <Text style={{textAlign : 'center'}}>12</Text>
          </View>
          <View style={{flex:1,}}>
            <Text style={{textAlign : 'center'}}>Nhịp tim gần nhất</Text>
            <Text style={{textAlign : 'center'}}>80</Text>
          </View>
        </View>
        <View style={{flexDirection:'column'}}>
          <Text style={{textAlign : 'center', alignItems: 'center', height:100}}>Bước chân</Text>
          <Text style={{textAlign : 'center', height: 100}}>Nhịp tim & SpO2</Text>
          <Text style={{textAlign : 'center', height: 100}}>Giấc ngủ (coming soon)</Text>
        </View>
      </View>
    );
  }
}

const mapStateToProps=(state) =>({
  selectedDeviceId: state.getIn(['ble', 'selectedDeviceId']),
  blestate: state.getIn(['ble', 'state']),
  devices: state.getIn(['ble', 'devices']),
  sceneState: state.getIn(['route', 'state'])

})

const mapDispatchToProps = {
  changeDeviceState: ble.changeDeviceState,
  selectService: ble.selectService
}

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(HomeComponent)
