A Web Bluetooth sample
IoTデバイス開発をしているとデバッグ目的やクライアントデモなどいろいろな理由でデバイス側からデータを飛ばす必要があります。デバイス側はnRF52シリーズのようなBLE機能のついたマイコン（BLE Nano V2に載ってるやつ）が多数出てきておりデバイスの選定には苦労しないのですが、受信側はデバッグ用アプリを書かなきゃいけなかったり、LightBlueのようなBLEデバッグアプリはデータ取得はカンタンですが、デモには適していなかったりと、サクッと作るには面倒なことが多いですね。
PCやMacのChromeブラウザやAndroidのChromeからはBluetoothにアクセスできるWeb Bluetoothが実装されています。これだとJSだけでカンタンにデモ・デバッグが出来ます。コンパイルも不要なのでセットアップ、イテレーションがかなり簡略化されます。デモプロジェクトをつくってみましたのでご参考にして頂ければ。
グラフはD3.jsで描きました。初D3で全部HTMLにJS平打ちなので見にくいのは勘弁してください。

When you develop IoT devices, there are some occations you need to fetch data from the device for debugging or demo for your client. For the device side we got many options like nRF52 series (the controller on the BLE Nano V2). However for the receiver side you often need to write up your own debug app or using BLE exisiting debug app like LightBlue. Writing own app is pain, and using debug app is not always neat way to present to someone like your boss. So I wrote demo project for WebBluetooth, which is implemented in PC/Mac's Chrome as well as Android's Chrome. Android's standard browser also support this feature. When you use WebBluetooth, you only need to write bit of JS and HTML and it runs already on your PC and Android's browser!

For the supoort status of Web Bluetooth on different platforms
https://caniuse.com/#search=web%20bluetooth
Chart was drawn by D3.js. It was my first time using D3.js. Sorry for very unreadable cord.
D3 chart was referenced from here:
http://bl.ocks.org/d3noob/7030f3D3

Source:
https://developers.google.com/web/updates/2015/07/interact-with-ble-devices-on-the-web
https://googlechrome.github.io/samples/web-bluetooth/notifications.html

Attached bleNano demo project can be conpiled for bleNano2 from Red Bear Lab