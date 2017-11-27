// ADeviceComponent.js
// 
 
 import React, { Component } from 'react'
 import { View, Text, StyleSheet, TouchableOpacity } from 'react-native'
import { connect } from 'react-redux'
import * as SceneConst from '../scene/Const.js'
import { Actions } from 'react-native-router-flux'
import * as ble from '../ble/BleActions'
import Style from '../view/Style'

class ADeviceComponent extends Component {
 	
 	_onPressButtonDisconnectDevice=()=>{
 		this.props.changeDeviceState(this.props.selectedDeviceId, ble.DEVICE_STATE_DISCONNECT);
 		Actions.pop();
 	}

 	render() {
 		return (
 			<View style={Style.component}>
 				<TouchableOpacity onPress={this._onPressButtonDisconnectDevice}>
 				<Text> Disconnect Device </Text>
	      </TouchableOpacity>
 			</View>
 		);
 	}
 }
 
const mapStateToProps=(state) =>({
	selectedDeviceId: state.getIn(['ble', 'selectedDeviceId']),
  sceneState: state.getIn(['route', 'state'])
})

const mapDispatchToProps = {
	changeDeviceState: ble.changeDeviceState
}

 export default connect(mapStateToProps, mapDispatchToProps)(ADeviceComponent)
