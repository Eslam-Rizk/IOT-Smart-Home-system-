import 'package:flutter/material.dart';
import 'package:bbbb/pages/design.dart';
import 'dart:developer';
import 'package:weather/weather.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter_icons/flutter_icons.dart';
import 'dart:io';
import 'package:get_ip/get_ip.dart';
import 'package:udp/udp.dart';
import 'package:path_provider/path_provider.dart';
import 'dart:convert';
import 'package:flutter/services.dart';
import 'package:intl/intl.dart';
import 'dart:async';
import 'package:bbbb/pages/designText.dart';

void main() {
  return runApp(MaterialApp(home: Home()));
}

class Home extends StatefulWidget {
  final Design design;

  const Home({Key key, this.design}) : super(key: key);

  @override
  _HomeState createState() => _HomeState();
}

class _HomeState extends State<Home> {
  //cache variables
  File jsonFile;
  Directory dir;
  String fileName = "myFile.json";
  bool fileExists = false;
  Map<String, dynamic> fileContent;

  String destination = 'xx.xx.xx.xx';
  String roomName, key = 'YourLocationKey', city = 'YourCityName';
  String tempTime = '', temperature = 'x', temperatureTime = '...';
  int weatherCode = 0;

  WeatherFactory wf;
  Weather w;
  double temp;
  Icon weatherIcon;
  Timer _timer;

  dynamic time = DateFormat.jm().format(DateTime.now());
  _triggerUpdate() {
    try {
      _timer = Timer.periodic(
          Duration(seconds: 1),
              (Timer timer) => (time != DateFormat.jm().format(DateTime.now()))
              ? setState(() {
            time = DateFormat.jm().format(DateTime.now());
          })
              : null);
    } catch (e) {
      print('erorrrrr: $e');
    }
  }

  @override
  void initState() {
    super.initState();
    _triggerUpdate();
    getApplicationDocumentsDirectory().then((Directory directory) {
      dir = directory;
      jsonFile = new File(dir.path + "/" + fileName);
      fileExists = jsonFile.existsSync();
      if (fileExists) {
        this.setState(() => fileContent = json.decode(jsonFile.readAsStringSync()));
        weatherCode = fileContent['condition'];
        getIcon();
        getTemp();
        temperature = fileContent['temp'];
        destination = fileContent['ip'].toString();
        log('home' + fileContent.toString());
      } else {
        print('file does not exist');
        destination = 'xx.xx.xx.xx';
        createFile({
          'ip': 'xx.xx.xx.xx',
          'temp': '25',
          'condition': 800,
          'hour': 12,
          'sunrise': 6,
          'sunset': 18,
        }, dir, fileName);
        temperature = '25';
      }
    });
  }

  void getIcon() {
    if (weatherCode >= 200 && weatherCode < 300) {
      if (fileContent['hour'] >= fileContent['sunset'] || fileContent['hour'] < fileContent['sunrise']) {
        setState(() {
          weatherIcon = Icon(
            WeatherIcons.wi_night_thunderstorm,
            color: Colors.black.withOpacity(.5),
            size: 30.0,
          );
        });
      } else {
        setState(() {
          weatherIcon = Icon(
            WeatherIcons.wi_day_thunderstorm,
            color: Colors.black.withOpacity(.5),
            size: 30.0,
          );
        });
      }
    } else if (weatherCode >= 300 && weatherCode < 400) {
      setState(() {
        weatherIcon = Icon(
          WeatherIcons.wi_raindrops,
          color: Colors.black.withOpacity(.5),
          size: 30.0,
        );
      });
    } else if (weatherCode >= 500 && weatherCode < 600) {
      if (fileContent['hour'] >= fileContent['sunset'] || fileContent['hour'] < fileContent['sunrise']) {
        setState(() {
          weatherIcon = Icon(
            WeatherIcons.wi_night_rain,
            color: Colors.black.withOpacity(.5),
            size: 30.0,
          );
        });
      } else {
        setState(() {
          weatherIcon = Icon(
            WeatherIcons.wi_day_rain,
            color: Colors.black.withOpacity(.5),
            size: 30.0,
          );
        });
      }
    } else if (weatherCode >= 600 && weatherCode < 700) {
      if (fileContent['hour'] >= fileContent['sunset'] || fileContent['hour'] < fileContent['sunrise']) {
        setState(() {
          weatherIcon = Icon(
            WeatherIcons.wi_night_snow,
            color: Colors.black.withOpacity(.5),
            size: 30.0,
          );
        });
      } else {
        setState(() {
          weatherIcon = Icon(
            WeatherIcons.wi_day_snow,
            color: Colors.black.withOpacity(.5),
            size: 30.0,
          );
        });
      }
    } else if (weatherCode >= 700 && weatherCode < 800) {
      if (fileContent['hour'] >= fileContent['sunset'] || fileContent['hour'] < fileContent['sunrise']) {
        setState(() {
          weatherIcon = Icon(
            WeatherIcons.wi_night_fog,
            color: Colors.black.withOpacity(.5),
            size: 30.0,
          );
        });
      } else {
        setState(() {
          weatherIcon = Icon(
            WeatherIcons.wi_day_fog,
            color: Colors.black.withOpacity(.5),
            size: 30.0,
          );
        });
      }
    } else if (weatherCode == 800) {
      if (fileContent['hour'] >= fileContent['sunset'] || fileContent['hour'] < fileContent['sunrise']) {
        setState(() {
          weatherIcon = Icon(
            WeatherIcons.wi_night_clear,
            color: Colors.black.withOpacity(.5),
            size: 30.0,
          );
        });
      } else {
        setState(() {
          weatherIcon = Icon(
            WeatherIcons.wi_day_sunny,
            color: Colors.black.withOpacity(.5),
            size: 30.0,
          );
        });
      }
    } else if (weatherCode >= 801 && weatherCode <= 804) {
      if (fileContent['hour'] >= fileContent['sunset'] || fileContent['hour'] < fileContent['sunrise']) {
        setState(() {
          weatherIcon = Icon(
            WeatherIcons.wi_night_cloudy,
            color: Colors.black.withOpacity(.5),
            size: 30.0,
          );
        });
      } else {
        setState(() {
          weatherIcon = Icon(
            WeatherIcons.wi_day_cloudy,
            color: Colors.black.withOpacity(.5),
            size: 30.0,
          );
        });
      }
    }
  }

  void getTemp() async {
    try {
      wf = new WeatherFactory(key);
    } catch (e) {
      print('weather factory error $e');
    }
    try {
      w = await wf.currentWeatherByLocation(31.04, 31.38);
    } catch (e) {
      print('weather parsing error--> $e');
    }
    if (w != null) {
      temp = w.temperature.celsius;
      if (temp.round().toString() != temperature) {
        writeToFile('temp', temp.round().toString());
        temperature = temp.round().toString();
      }
      weatherCode = w.weatherConditionCode;
      writeToFile('hour', w.date.hour);
      writeToFile('sunrise', w.sunrise.hour);
      writeToFile('sunset', w.sunset.hour);
      writeToFile('tempTime', 'Updated ' + DateTime.now().day.toString() + "/" + DateTime.now().month.toString() + " " + "${DateFormat.jm().format(DateTime.now())}");
      print(w);
      getIcon();
      temperatureTime = fileContent['tempTime'];
    }
    weatherCode == 0
        ? setState(() {
      fileExists
          ? weatherCode = fileContent['condition']
          : weatherIcon = Icon(
        WeatherIcons.wi_day_sunny_overcast,
        color: Colors.black.withOpacity(.5),
        size: 30.0,
      );
    })
        : writeToFile('condition', weatherCode);
    print(weatherCode);
    if (weatherCode != 0) {
      print('weatherCode!=0');
    }
    if (temp != null) {
      if (temp.round().toString() != fileContent['temp']) {
        writeToFile('temp', temp.round().toString());
        temperature = temp.round().toString();
      }
    }
    tempTime = fileContent['tempTime'].toString();
    temperatureTime = fileContent['tempTime'];
    if (temp == null) {
      writeToFile('temp', '25');
    }
  }

  @override
  void dispose() {
    _timer.cancel();
    super.dispose();
  }

  void createFile(Map<String, dynamic> content, Directory dir, String fileName) {
    print('creating file!');
    File file = new File(dir.path + "/" + fileName);
    file.createSync();
    file.writeAsStringSync(json.encode(content));
    fileExists = true;
    getTemp();
  }

  void writeToFile(String key, dynamic value) {
    print('Writing to file!');
    Map<String, dynamic> content = {
      key: value
    };
    if (fileExists) {
      print('file exists');
      Map<String, dynamic> jsonFileContent = json.decode(jsonFile.readAsStringSync());
      jsonFileContent.addAll(content);
      jsonFile.writeAsStringSync(json.encode(jsonFileContent));
    } else {
      print('file does not exist');
      createFile(content, dir, fileName);
    }
    this.setState(() => fileContent = json.decode(jsonFile.readAsStringSync()));
  }

  String name1 = '1';
  String name2 = '2';
  String name3 = '3';
  String name4 = '4';
  String name5 = '5';

  var scaffoldKey = GlobalKey<ScaffoldState>();

  @override
  Widget build(BuildContext context) {
    SystemChrome.setEnabledSystemUIOverlays([]);
    return Scaffold(
      //resizeToAvoidBottomPadding: false,
      backgroundColor: Color(0xFFe6ebf2),
      body: Padding(
        padding: EdgeInsets.symmetric(horizontal: 18.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: <Widget>[
            SizedBox(
              height: MediaQuery.of(context).viewPadding.top + 10,
            ),
            Container(
              height: 30,
              child: Center(
                child: Icon(
                  Icons.home,
                  size: 30,
                  color: Colors.black.withOpacity(0.5),
                ),
              ),
            ), //my home
            SizedBox(
              height: MediaQuery.of(context).copyWith().size.height * 0.025,
            ),
            Row(
              children: [
                Container(
                  height: MediaQuery.of(context).copyWith().size.height * 0.11,
                  width: MediaQuery.of(context).copyWith().size.width * 0.44,
                  decoration: BoxDecoration(color: Color(0xFFe6ebf2), borderRadius: BorderRadius.all(Radius.circular(20.0)), boxShadow: [
                    BoxShadow(blurRadius: 5.0, offset: Offset(-3, -3), color: Colors.white.withOpacity(.7)),
                    BoxShadow(blurRadius: 5.0, offset: Offset(3, 3), color: Colors.black.withOpacity(.15))
                  ]),
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.center,
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      //SizedBox(height: 15),
                      Row(
                        crossAxisAlignment: CrossAxisAlignment.center,
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: <Widget>[
                          //SizedBox(width: 15,),
                          Container(
                            height: 40,
                            width: 55,
                            child: weatherCode == 0
                                ? Icon(
                              WeatherIcons.wi_day_sunny_overcast,
                              color: Colors.black.withOpacity(.5),
                              size: 30.0,
                            )
                                : weatherIcon, //weatherIcon,
                          ),
                          SizedBox(
                            width: MediaQuery.of(context).copyWith().size.width * 0.05,
                          ),
                          Center(
                            child: Text(temperature != 'x' ? '$temperature °C' : '25 °C',
                                style: TextStyle(
                                  color: Colors.black.withOpacity(0.5),
                                  fontSize: 22.0,
                                  fontWeight: FontWeight.bold,
                                )),
                          ),
                        ],
                      ),
                      Row(
                        crossAxisAlignment: CrossAxisAlignment.end,
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: [
                          Container(
                              width: MediaQuery.of(context).copyWith().size.width * 0.25,
                              child: Text(
                                temperatureTime != '...' ? temperatureTime : '...',
                                style: TextStyle(fontSize: 12, color: Colors.black.withOpacity(0.7)),
                              )),
                          //SizedBox(width: 5,),
                          InkWell(
                            onTap: () {
                              //sound.play('waterdrop.wav');
                              setState(() {
                                getTemp();
                              });
                            },
                            splashColor: Colors.transparent,
                            focusColor: Colors.transparent,
                            child: Icon(
                              Icons.refresh,
                              size: 15,
                              color: Colors.black.withOpacity(0.7),
                            ),
                          ),
                        ],
                      ),
                    ],
                  ),
                ), //time
                SizedBox(
                  width: MediaQuery.of(context).copyWith().size.width * 0.02,
                ),
                Container(
                  height: MediaQuery.of(context).copyWith().size.height * 0.11,
                  width: MediaQuery.of(context).copyWith().size.width * 0.44,
                  decoration: BoxDecoration(color: Color(0xFFe6ebf2), borderRadius: BorderRadius.all(Radius.circular(20.0)), boxShadow: [
                    BoxShadow(blurRadius: 5.0, offset: Offset(-3, -3), color: Colors.white.withOpacity(.7)),
                    BoxShadow(blurRadius: 5.0, offset: Offset(3, 3), color: Colors.black.withOpacity(.15))
                  ]),
                  child: Container(
                    child: Row(
                      crossAxisAlignment: CrossAxisAlignment.center,
                      mainAxisAlignment: MainAxisAlignment.center,
                      children: <Widget>[
                        Container(
                          height: 40,
                          width: 40,
                          decoration: BoxDecoration(
                            color: Color(0xFFe6ebf2),
                            borderRadius: BorderRadius.all(Radius.circular(20.0)),
                          ),
                          child: Icon(
                            FontAwesome.clock_o,
                            color: Colors.black.withOpacity(.5),
                            size: 30.0,
                          ),
                        ),
                        /*SizedBox(
                          width: MediaQuery.of(context).copyWith().size.width * 0.01,
                        ),*/
                        Text(time,
                            style: TextStyle(
                              //fontFamily: "nunito",
                              color: Colors.black.withOpacity(.5),
                              fontSize: 22.0,
                              fontWeight: FontWeight.bold,
                            )),
                      ],
                    ),
                  ),
                ), //time
              ],
            ), //temp & time
            SizedBox(
              height: MediaQuery.of(context).copyWith().size.height * 0.02,
            ),
            //main buttons
            Container(
              height: MediaQuery.of(context).copyWith().size.height * 0.75,
              width: double.infinity,
              decoration: BoxDecoration(color: Color(0xFFDCE7F1), borderRadius: BorderRadius.all(Radius.circular(20.0)), shape: BoxShape.rectangle, boxShadow: [
                BoxShadow(offset: Offset(2, 2), blurRadius: 3.0, color: Colors.white.withOpacity(.7)),
                BoxShadow(offset: Offset(-2, -2), blurRadius: 3.0, color: Colors.black.withOpacity(.15))
              ]),
              child: Padding(
                  padding: const EdgeInsets.all(2.0),
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.start,
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Row(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: [
                          Padding(
                            padding: const EdgeInsets.all(6),
                            child: InkWell(
                              onTap: () async {
                                //sound.play('waterdrop.wav');
                                String order = '1';
                                String ipAddress = await GetIp.ipAddress;
                                print(ipAddress);
                                var sender = await UDP.bind(Endpoint.unicast(InternetAddress(ipAddress), port: Port(4210)));
                                var dataLength = await sender.send(order.codeUnits, Endpoint.unicast(InternetAddress(destination), port: Port(4210)));
                                print("$order ($dataLength) bytes sent.");
                                sender.close();
                              },
                              child: Center(
                                child: DesignText(
                                  height1: MediaQuery.of(context).copyWith().size.height * 0.169,
                                  width1: MediaQuery.of(context).copyWith().size.width * 0.4,
                                  color: Color(0xFFe6ebf2),
                                  offsetB: Offset(-2, -2),
                                  offsetW: Offset(2, 2),
                                  bLevel: 3.0,
                                  text: 'D1',
                                  iconSize: 25.0,
                                  weight: FontWeight.bold,
                                  radius1: 20,
                                  radius2: 20,
                                  radius3: 20,
                                  radius4: 20,
                                  pressed: Colors.black.withOpacity(0.5),
                                ),
                              ),
                            ),
                          ),
                          SizedBox(
                            width: 7,
                          ),
                          Padding(
                            padding: const EdgeInsets.all(6),
                            child: InkWell(
                              onTap: () async {
                                //sound.play('waterdrop.wav');
                                String order = '2';
                                String ipAddress = await GetIp.ipAddress;
                                print(ipAddress);
                                var sender = await UDP.bind(Endpoint.unicast(InternetAddress(ipAddress), port: Port(4210)));
                                var dataLength = await sender.send(order.codeUnits, Endpoint.unicast(InternetAddress(destination), port: Port(4210)));
                                print("$order ($dataLength) bytes sent.");
                                sender.close();
                              },
                              child: Center(
                                child: DesignText(
                                  height1: MediaQuery.of(context).copyWith().size.height * 0.169,
                                  width1: MediaQuery.of(context).copyWith().size.width * 0.4,
                                  color: Color(0xFFe6ebf2),
                                  offsetB: Offset(-2, -2),
                                  offsetW: Offset(2, 2),
                                  bLevel: 3.0,
                                  text: 'D2',
                                  iconSize: 25.0,
                                  weight: FontWeight.bold,
                                  radius1: 20,
                                  radius2: 20,
                                  radius3: 20,
                                  radius4: 20,
                                  pressed: Colors.black.withOpacity(0.5),
                                ),
                              ),
                            ),
                          ),
                        ],
                      ),
                      SizedBox(
                        height: 3,
                      ),
                      Row(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: [
                          Padding(
                            padding: const EdgeInsets.all(6),
                            child: InkWell(
                              onTap: () async {
                                //sound.play('waterdrop.wav');
                                String order = '3';
                                String ipAddress = await GetIp.ipAddress;
                                print(ipAddress);
                                var sender = await UDP.bind(Endpoint.unicast(InternetAddress(ipAddress), port: Port(4210)));
                                var dataLength = await sender.send(order.codeUnits, Endpoint.unicast(InternetAddress(destination), port: Port(4210)));
                                print("$order ($dataLength) bytes sent.");
                                sender.close();
                              },
                              child: Center(
                                child: DesignText(
                                  height1: MediaQuery.of(context).copyWith().size.height * 0.169,
                                  width1: MediaQuery.of(context).copyWith().size.width * 0.4,
                                  color: Color(0xFFe6ebf2),
                                  offsetB: Offset(-2, -2),
                                  offsetW: Offset(2, 2),
                                  bLevel: 3.0,
                                  text: 'D3',
                                  iconSize: 25.0,
                                  weight: FontWeight.bold,
                                  radius1: 20,
                                  radius2: 20,
                                  radius3: 20,
                                  radius4: 20,
                                  pressed: Colors.black.withOpacity(0.5),
                                ),
                              ),
                            ),
                          ),
                          SizedBox(
                            width: 7,
                          ),
                          Padding(
                            padding: const EdgeInsets.all(6),
                            child: InkWell(
                              onTap: () async {
                                //sound.play('waterdrop.wav');
                                String order = '4';
                                String ipAddress = await GetIp.ipAddress;
                                print(ipAddress);
                                var sender = await UDP.bind(Endpoint.unicast(InternetAddress(ipAddress), port: Port(4210)));
                                var dataLength = await sender.send(order.codeUnits, Endpoint.unicast(InternetAddress(destination), port: Port(4210)));
                                print("$order ($dataLength) bytes sent.");
                                sender.close();
                              },
                              child: Center(
                                child: DesignText(
                                  height1: MediaQuery.of(context).copyWith().size.height * 0.169,
                                  width1: MediaQuery.of(context).copyWith().size.width * 0.4,
                                  color: Color(0xFFe6ebf2),
                                  offsetB: Offset(-2, -2),
                                  offsetW: Offset(2, 2),
                                  bLevel: 3.0,
                                  text: 'D4',
                                  iconSize: 25.0,
                                  weight: FontWeight.bold,
                                  radius1: 20,
                                  radius2: 20,
                                  radius3: 20,
                                  radius4: 20,
                                  pressed: Colors.black.withOpacity(0.5),
                                ),
                              ),
                            ),
                          ),
                        ],
                      ),
                      SizedBox(
                        height: 3,
                      ),
                      Row(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: [
                          Padding(
                            padding: const EdgeInsets.all(6),
                            child: InkWell(
                              onTap: () async {
                                //sound.play('waterdrop.wav');
                                String order = '5';
                                String ipAddress = await GetIp.ipAddress;
                                print(ipAddress);
                                var sender = await UDP.bind(Endpoint.unicast(InternetAddress(ipAddress), port: Port(4210)));
                                var dataLength = await sender.send(order.codeUnits, Endpoint.unicast(InternetAddress(destination), port: Port(4210)));
                                print("$order ($dataLength) bytes sent.");
                                sender.close();
                              },
                              child: Center(
                                child: DesignText(
                                  height1: MediaQuery.of(context).copyWith().size.height * 0.169,
                                  width1: MediaQuery.of(context).copyWith().size.width * 0.4,
                                  color: Color(0xFFe6ebf2),
                                  offsetB: Offset(-2, -2),
                                  offsetW: Offset(2, 2),
                                  bLevel: 3.0,
                                  text: 'D5',
                                  iconSize: 25.0,
                                  weight: FontWeight.bold,
                                  radius1: 20,
                                  radius2: 20,
                                  radius3: 20,
                                  radius4: 20,
                                  pressed: Colors.black.withOpacity(0.5),
                                ),
                              ),
                            ),
                          ),
                          SizedBox(
                            width: 7,
                          ),
                          Padding(
                            padding: const EdgeInsets.all(6),
                            child: InkWell(
                              onTap: () async {
                                //sound.play('waterdrop.wav');
                                String order = '6';
                                String ipAddress = await GetIp.ipAddress;
                                print(ipAddress);
                                var sender = await UDP.bind(Endpoint.unicast(InternetAddress(ipAddress), port: Port(4210)));
                                var dataLength = await sender.send(order.codeUnits, Endpoint.unicast(InternetAddress(destination), port: Port(4210)));
                                print("$order ($dataLength) bytes sent.");
                                sender.close();
                              },
                              child: Center(
                                child: DesignText(
                                  height1: MediaQuery.of(context).copyWith().size.height * 0.169,
                                  width1: MediaQuery.of(context).copyWith().size.width * 0.4,
                                  color: Color(0xFFe6ebf2),
                                  offsetB: Offset(-2, -2),
                                  offsetW: Offset(2, 2),
                                  bLevel: 3.0,
                                  text: 'D6',
                                  iconSize: 25.0,
                                  weight: FontWeight.bold,
                                  radius1: 20,
                                  radius2: 20,
                                  radius3: 20,
                                  radius4: 20,
                                  pressed: Colors.black.withOpacity(0.5),
                                ),
                              ),
                            ),
                          ),
                        ],
                      ),
                      SizedBox(
                        height: 3,
                      ),
                      Row(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: [
                          Padding(
                            padding: const EdgeInsets.all(6),
                            child: InkWell(
                              onTap: () async {
                                //sound.play('waterdrop.wav');
                                String order = '7';
                                String ipAddress = await GetIp.ipAddress;
                                print(ipAddress);
                                var sender = await UDP.bind(Endpoint.unicast(InternetAddress(ipAddress), port: Port(4210)));
                                var dataLength = await sender.send(order.codeUnits, Endpoint.unicast(InternetAddress(destination), port: Port(4210)));
                                print("$order ($dataLength) bytes sent.");
                                sender.close();
                              },
                              child: Center(
                                child: DesignText(
                                  height1: MediaQuery.of(context).copyWith().size.height * 0.169,
                                  width1: MediaQuery.of(context).copyWith().size.width * 0.4,
                                  color: Color(0xFFe6ebf2),
                                  offsetB: Offset(-2, -2),
                                  offsetW: Offset(2, 2),
                                  bLevel: 3.0,
                                  text: 'D7',
                                  iconSize: 25.0,
                                  weight: FontWeight.bold,
                                  radius1: 20,
                                  radius2: 20,
                                  radius3: 20,
                                  radius4: 20,
                                  pressed: Colors.black.withOpacity(0.5),
                                ),
                              ),
                            ),
                          ),
                          SizedBox(
                            width: 7,
                          ),
                          Padding(
                            padding: const EdgeInsets.all(6),
                            child: InkWell(
                              onTap: () async {
                                //sound.play('waterdrop.wav');
                                String order = '8';
                                String ipAddress = await GetIp.ipAddress;
                                print(ipAddress);
                                var sender = await UDP.bind(Endpoint.unicast(InternetAddress(ipAddress), port: Port(4210)));
                                var dataLength = await sender.send(order.codeUnits, Endpoint.unicast(InternetAddress(destination), port: Port(4210)));
                                print("$order ($dataLength) bytes sent.");
                                sender.close();
                              },
                              child: Center(
                                child: DesignText(
                                  height1: MediaQuery.of(context).copyWith().size.height * 0.169,
                                  width1: MediaQuery.of(context).copyWith().size.width * 0.4,
                                  color: Color(0xFFe6ebf2),
                                  offsetB: Offset(-2, -2),
                                  offsetW: Offset(2, 2),
                                  bLevel: 3.0,
                                  text: 'D8',
                                  iconSize: 25.0,
                                  weight: FontWeight.bold,
                                  radius1: 20,
                                  radius2: 20,
                                  radius3: 20,
                                  radius4: 20,
                                  pressed: Colors.black.withOpacity(0.5),
                                ),
                              ),
                            ),
                          ),
                        ],
                      ),
                    ],
                  ),
                ),
              ), //quick access
          ],
        ),
      ),
    );
  }
}
