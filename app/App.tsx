import * as React from 'react';
import { DefaultTheme, Provider as PaperProvider , Appbar } from 'react-native-paper';
import { AppRegistry } from 'react-native';
let appName = 'Induti';


const theme = {
  ...DefaultTheme,
  colors: {
    ...DefaultTheme.colors,
    primary: 'tomato',
    accent: 'yellow',
  },
};

export default function Main() {

  const _connection = () => console.log('Shown more');
  return (
    <PaperProvider theme={theme}>
    <Appbar.Header>
      <Appbar.Content title="Induti" subtitle="Subtitle" />
      <Appbar.Action icon="access-point" onPress={_connection} />
    </Appbar.Header>

    </PaperProvider>
  );
}


AppRegistry.registerComponent(appName, () => Main);