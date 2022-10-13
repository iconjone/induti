import * as React from 'react';
import { DefaultTheme, Provider as PaperProvider , Appbar, Headline } from 'react-native-paper';
import { AppRegistry } from 'react-native';
let appName = 'Induti';

import { IndutiWebSocketControl } from 'induti-websocket-control';





const theme = {
  ...DefaultTheme,
  colors: {
    ...DefaultTheme.colors,
    primary: 'tomato',
    accent: 'yellow',
  },
};

export default function Main() {
  const [isConnected, setIsConnected] = React.useState(false);
  const [indutiState, setIndutiState] = React.useState({IH: 0, LED: 0, CONTACT: false, TEMP: 0});
  let induti=React.useRef(new IndutiWebSocketControl("induti.local")).current
const handleIndutiStateChange = (indutiState: {IH: number, LED: number, TEMP: number, CONTACT: boolean}) => {
  // console.log(indutiState);
  setIndutiState({...indutiState});
}
  const _connection = () => console.log('Do some steps to connect induti to wireless');
//via useEffect on induti data event update state
  React.useEffect(() => {
    induti.event.on('data',(data: any)=>{
      console.log(data)
      // props.state.IH=data.IH
      // handleIndutiStateChange(data)
    })
    induti.event.on('connection', (connectionState: boolean) =>{
      //refresh react status
      setIsConnected(connectionState)
    })
  },[induti])
  
  return (
    <PaperProvider theme={theme}>
    <Appbar.Header>
      <Appbar.Content title="Induti" subtitle={isConnected ? "link" : "link-off"} />
      <Appbar.Action icon={isConnected ? "link" : "link-off"} onPress={_connection} />
    </Appbar.Header>
<Headline>{indutiState.TEMP}</Headline>
    </PaperProvider>
  );
}


AppRegistry.registerComponent(appName, () => Main);