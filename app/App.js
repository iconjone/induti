import React from 'react';
import { Switch, HStack, Text, Center, NativeBaseProvider } from "native-base"

function App() {
  ip= "192.168.1.120"

  return (
    <NativeBaseProvider>
    <Center flex={1}>
    <HStack alignItems="center" space={8}>
      <Text fontSize="lg">Setting 1</Text>
      <Switch size="lg" onToggle={(value)=>console.log(value)} / >
      {/* fetch("http://" + ip +"/"+(value?"off":"on")) */}
    </HStack>
   </Center>
    </NativeBaseProvider>
);}


export default App;
