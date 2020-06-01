/*
 Page html avec le CSS
 La page html est stockée en mémoire flash afin d'économiser de la RAM
 */

const char MAIN_page[] PROGMEM = R"=====(
<html>
    <head>
    
        <title>TODO supply a title</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <style>
/*
 * pc

@media only screen and (max-width: 600px)
{

}

.square {
  height: 141px;
  width: 1343px;
  background-color: #33beff;
  position: absolute;
}

#tsquare
{
  color: white;
  font-family: verdana;
  font-size: 400%; 
  position: absolute;
  left: 174px;
} 

#tsquare2
{
  color: white;
  font-family: verdana;
  font-size: 400%; 
  position: absolute;
  left: 575px;
} 

#tsquare3
{
  color: white;
  font-family: verdana;
  font-size: 400%; 
  position: absolute;
  left: 940px;
} 
        
.square2 {
  height: 964px;
  width: 1343px;
  background-color: #e8e8e8;
  position: absolute;
  top:150px;
}

.square3 {
  height: 160px;
  width: 1343px;
  background-color: #33beff;
  position: absolute;
  top:950px;
}

#tTemp
{
  color: black;
  font-family: verdana;
  font-size: 300%; 
  position: absolute;

}

#Temp
{
  color: red;
  font-family: verdana;
  font-size: 100%; 
  position: absolute;
}

#tPress
{
  color: black;
  font-family: verdana;
  font-size: 300%; 
  position: absolute;
  top: 100px;

}

#Pression
{
  color: red;
  font-family: verdana;
  font-size: 100%; 
  position: absolute;
}

#tHum
{
  color: black;
  font-family: verdana;
  font-size: 300%; 
  position: absolute;
  top: 200px;
}

#Hum
{
  color: red;
  font-family: verdana;
  font-size: 100%; 
  position: absolute;
}


#tRad
{
  color: black;
  font-family: verdana;
  font-size: 300%; 
  position: absolute;
  top: 300px;
}

#Rad
{
  color: red;
  font-family: verdana;
  font-size: 100%; 
  position: absolute;
}

#tPosLa
{
  color: black;
  font-family: verdana;
  font-size: 300%; 
  position: absolute;
  top: 400px;
}

#tPosLo
{
  color: black;
  font-family: verdana;
  font-size: 300%; 
  position: absolute;
  top: 500px;
}

#Pos
{
  color: red;
  font-family: verdana;
  font-size: 100%; 
  position: absolute;
}

#tAlt
{
  color: black;
  font-family: verdana;
  font-size: 300%; 
  position: absolute;
  top: 600px;
}

#Alt
{
  color: red;
  font-family: verdana;
  font-size: 100%; 
  position: absolute;
}

#tnbM
{
  color: black;
  font-family: verdana;
  font-size: 300%; 
  position: absolute;
  top: 700px;
}

#tbutton
{
  position: absolute;
  top:12%;
  left: 40%;
  font-size: 500%; 
}
 */

.square {
  height: 141px;
  width: 1343px;
  background-color: #33beff;
  position: absolute;
}

#tsquare
{
  color: white;
  font-family: verdana;
  font-size: 400%; 
  position: absolute;
  left: 174px;
} 

#tsquare2
{
  color: white;
  font-family: verdana;
  font-size: 400%; 
  position: absolute;
  left: 575px;
} 

#tsquare3
{
  color: white;
  font-family: verdana;
  font-size: 400%; 
  position: absolute;
  left: 940px;
} 
        
.square2 {
  height: 964px;
  width: 1343px;
  background-color: #e8e8e8;
  position: absolute;
  top:150px;
}

.square3 {
  height: 160px;
  width: 1343px;
  background-color: #33beff;
  position: absolute;
  top:975px;
}

#tTemp
{
  color: black;
  font-family: verdana;
  font-size: 400%; 
  position: absolute;

}

#Temp
{
  color: red;
  font-family: verdana;
  font-size: 100%; 
  position: absolute;
}

#tPress
{
  color: black;
  font-family: verdana;
  font-size: 400%; 
  position: absolute;
  top: 100px;

}

#Pression
{
  color: red;
  font-family: verdana;
  font-size: 100%; 
  position: absolute;
}

#tHum
{
  color: black;
  font-family: verdana;
  font-size: 400%; 
  position: absolute;
  top: 200px;
}

#Hum
{
  color: red;
  font-family: verdana;
  font-size: 100%; 
  position: absolute;
}


#tRad
{
  color: black;
  font-family: verdana;
  font-size: 400%; 
  position: absolute;
  top: 300px;
}

#Rad
{
  color: red;
  font-family: verdana;
  font-size: 100%; 
  position: absolute;
}

#tPosLa
{
  color: red;
  font-family: verdana;
  font-size: 400%; 
  position: absolute;
  top: 400px;
}

#tPosLo
{
  color: red;
  font-family: verdana;
  font-size: 400%; 
  position: absolute;
  top: 500px;
}

#Pos
{
  color: red;
  font-family: verdana;
  font-size: 100%; 
  position: absolute;
}

#tAlt
{
  color: black;
  font-family: verdana;
  font-size: 400%; 
  position: absolute;
  top: 600px;
}

#Alt
{
  color: red;
  font-family: verdana;
  font-size: 100%; 
  position: absolute;
}

#tnbM
{
  color: black;
  font-family: verdana;
  font-size: 400%; 
  position: absolute;
  top: 700px;
}

#tbutton
{
  position: absolute;
  top:12%;
  left: 40%;
  font-size: 500%; 
}
        
        </style>
        <script>
        var compteur=0;
            setInterval(function () {
              compteur = compteur +1;
              if(compteur == 10)// le boutton est réactivé après 20 secondes (évite d'envoyer un autre trame pendant le traitement)
              {                
                document.getElementById("tbutton").disabled = false;
              }
                getData();
            }, 2000); //mise à jour toutes les 2 Secondes de la page web avec la fonction getData
            function getData() {
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function () {
                    if (this.readyState == 4 && this.status == 200) {
                        //document.getElementById("Value").innerHTML = this.responseText;
                        var str = this.responseText;
                        var values = str.split(";");            //les champs sont séparés par une virgule
                        document.getElementById("Temp").innerHTML =
                                values[0];
                        document.getElementById("Pression").innerHTML =
                                values[1];
                         document.getElementById("Hum").innerHTML =
                                values[2];
                        document.getElementById("Rad").innerHTML =
                                values[3];
                        document.getElementById("PosLat").innerHTML =
                                values[4];
                        document.getElementById("PosLong").innerHTML =
                                values[5];
                        document.getElementById("Alt").innerHTML =
                                values[6];
                    }
                };
                xhttp.open("GET", "Value", true);     //envoie Value au serveur WEB avec la methode GET
                xhttp.send();
            }
            function sendCmd() {                      //fonction lors d'un appui sur le bouton d'envoi
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function () {
                    if (this.readyState == 4 && this.status == 200) {   //recupere le nombre de messages envoyés
                        document.getElementById("Envoi").innerHTML = this.responseText;
                        document.getElementById("tbutton").disabled = true;   //desactive le bouton 
                        compteur = 0;
                    }
                };
                xhttp.open("GET", "Sigfox", true);    //envoie Sigfox au serveur WEB avec la methode GET
                xhttp.send();
            }
        </script>
    </head>
    <body>
    <div class="square">
    <h1 id="tsquare">BALLON</h1>
    <h1 id="tsquare2">SONDE</h1>
    <h1 id="tsquare3">2020</h1>
    </div>
    <div class="square2" >
        <h2 id="tTemp"> Température(°C): <span id="Temp"></span></h2> 
        <h2 id="tPress">Pression(hPa): <span id="Pression"></span></h2> 
        <h2 id="tHum">Humidite(%): <span id="Hum"></span></h2> 
        <h2 id="tRad">Radiation(uSvh): <span id="Rad"></span></h2> 
        <h2 id="tPosLa">Latitude(Deg Dec): <span id="PosLat"></span></h2> 
        <h2 id="tPosLo">Longitude(Deg Dec): <span id="PosLong"></span></h2> 
        <h2 id="tAlt">Altitude(m): <span id="Alt"></span></h2> 
        <h1 id="tnbM" >Nb de message envoyés manuellement: <span id="Envoi">0</span> </h1>
    </div>
    <div class="square3">
    <button type="button" onclick="sendCmd()" id="tbutton">Sigfox</button>
    </div>
</body>
</html>
)=====";
