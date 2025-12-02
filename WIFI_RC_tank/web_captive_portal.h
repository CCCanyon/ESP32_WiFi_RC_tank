const char PAGE_REDIRECT_0[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Captive Portal Login</title>
  <style>
    body {
      margin: 0;
      padding: 0;
      border: 0;
      display: flex;
      flex-direction: column;
      overflow-x: hidden;
      min-height: 100vh;
      background-color: #DDD;
      font-family: sans-serif;
      color: #555;
    }
    
    .gray-note {
      color: #777;
    }

    .block {
      margin: 0;
      border: 0;
      padding: 0;
      width: 100%;
      text-align: center;
    }

    .login-container, .white-box {
      margin: 10px;
      margin-left: 10vw;
      margin-right: 10vw;
      border-radius: 8px;
      padding: 20px;
      text-align: center;
      font-size: 18px;
      
      background-color: #fff;
    }

    h2 {
      text-align: left;
      font-size: 24px;
      margin: 0;
      margin-bottom: 20px;
      color: #333;
    }

    .login-container label {
      display: block;
      text-align: left;
      margin-bottom: 5px;
      color: #555;
    }

    .login-container input[type="text"],
    .login-container input[type="password"] {
      width: calc(100% - 20px);
      padding: 10px;
      margin-bottom: 15px;
      border: 1px solid #ddd;
      border-radius: 4px;
      box-sizing: border-box;
    }

    .login-container input[type="submit"], a, .button {
      box-sizing: border-box;
      display: inline-block;
      background-color: #07F;
      color: white;
      padding: 10px 15px;
      border: none;
      border-radius: 16px;
      cursor: pointer;
      font-size: 16px;
      width: calc(100% - 60px);
      text-decoration: none;
      text-align: center;
    }

    .ip-box {
      font-size: 6vw;
      font-weight: bold;
      background-color: #EEE;
      padding: 10px;
      border-radius: 16px;
    }

  </style>
</head>

<body>
  <div class="block">
    <div class="white-box" style="background-color: #FE9;">
      <h2 class="gray-note">Option 1: Connect To</h2>
      <p class="gray-note"><b>Disable mobile data & open this IP in browser:</b></p>
      <div class="ip-box">192.168.1.1</div>
      <p class="gray-note">Hold-select open, or copy to/type in the adress bar.</p>
    </div>
  </div>
  <div class="block">
    <div class="white-box">
      <h2> Option 2: Via Router or Hotspot</h2>)=====";

const char PAGE_REDIRECT_1_CONNECTED_0[] PROGMEM = R"=====(
      <p><b>Open this IP in browser & switch to the same WiFi network down below:</b></p>
      <div class="ip-box">)=====";
const char PAGE_REDIRECT_1_CONNECTED_1[] PROGMEM = R"=====(</div>
      <p>Hold-select open, or copy to/type in the adress bar.</p>)=====";

const char PAGE_REDIRECT_1_NOT_CONNECTED[] PROGMEM = R"=====(
      <div class="block">
        <div class="white-box" style="background-color: #CCC;">
          <p>Not yet connected. Try refreshing!</p>
        </div>
      </div>)=====";

const char PAGE_REDIRECT_2[] PROGMEM = R"=====(
    </div>
  </div>
  <div class="block">
    <div class="white-box">
      <a href="">Refresh Page</a>
    </div>
  </div>
  <div class="block">
    <div class="login-container">
      <h2>Configure WiFi:</h2>
      <form method="post" action="/ap-update">
        <label for="SSID">Network:</label>
        <input type="text" id="SSID" name="network" value=")=====";

const char PAGE_REDIRECT_3[] PROGMEM = R"=====(" required>
        <label for="password">Password:</label>
        <input type="password" id="password" name="password" value="" required>
        <input type="submit" value="Update Changes">
      </form>
    </div>
  </div>
</body>

</html>)=====";