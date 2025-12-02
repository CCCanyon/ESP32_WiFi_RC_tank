const char HTML_OK[] PROGMEM = R"=====(

)=====";

const char SNULL[] PROGMEM = "";

const char PAGE_HOME[] PROGMEM = R"=====(HTTP/1.1 200 OK Content-type:text/html<!doctypehtml><html lang=en><title>PLAYER</title><link href="data:;base64,="rel=icon><meta charset=UTF-8><meta content="width=device-width,initial-scale=1"name=viewport><meta content=yes name=mobile-web-app-capable><link href=manifest.json rel=manifest><style>body{margin:0;padding:0;border:0;display:flex;flex-direction:column;overflow-x:hidden;min-height:100vh;background-color:#ddd;font-family:sans-serif;color:#555}.block{margin:0;border:0;padding:0;width:100%;text-align:center}h1,h2,p{margin:5px;border:0;overflow-wrap:break-word;text-align:center;font-size:20px}h1{font-size:30px}h2{font-size:24px}small{font-size:18px}.cred-container,.white-box{margin:10px;margin-left:10vw;margin-right:10vw;border-radius:8px;padding:20px;text-align:center;font-size:18px;background-color:#eee;position:relative;z-index:0}.button,.k-slider{margin-top:5px;margin-bottom:10px;box-sizing:border-box;display:inline-block;background-color:#07f;color:#fff;padding:5px;border-radius:16px;cursor:pointer;font-size:18px;width:calc(100% - 20px);text-decoration:none;text-align:center}.symbol{width:50px;height:50px;align-items:center;justify-content:center;margin:1px;margin-top:10px;margin-bottom:10px;background:#555;color:#fff;border-radius:10px;position:relative;display:inline-flex}.navbar{z-index:1;margin:0;overflow:hidden;background-color:#333;position:fixed;top:0;width:100%}.navbar a{float:left;display:block;color:#f2f2f2;text-align:center;padding:14px 16px;font-size:17px}.turq{color:#eee;background-color:#2ba}.ylw{color:#222;background-color:#fe3}.black{color:#eee;background-color:#222}.red{color:#eee;background-color:#f26}.white{color:#555;background-color:#fff}.greenbar,.redbar{z-index:1;margin:0;overflow:hidden;color:#ddd;position:fixed;bottom:0;width:100%;transform:translateY(100%);transition:transform .4s ease-in-out}.greenbar.visible,.redbar.visible{transform:translateY(0)}.redbar{background-color:#f26}.greenbar{background-color:#2b3}.hide{opacity:0;pointer-events:none;height:0;overflow:hidden}.k-slider{user-select:none;color:#222;background-color:#fff}.blink{animation:turqBlink 1s}.cred-container label{display:block;text-align:left;margin-bottom:5px;color:#555}.cred-container input{width:calc(100% - 20px);padding:10px;margin-bottom:15px;border:4px solid #ccc;background-color:#fff;border-radius:4px;box-sizing:border-box}.ip-box{font-size:20px;margin:10px;font-weight:700;background-color:#ddd;padding:10px;border-radius:16px}@keyframes turqBlink{25%{background-color:#2ba}50%{background-color:#2ba}}@keyframes rotating{0%{color:#ddd}25%{color:#2ba}50%{color:#222}75%{color:#fe3}100%{color:#ddd}}.dotdot::after{content:"\2234";font-weight:900;font-size:24px;animation:rotating 2s linear infinite}.stop-scrolling{height:100%;overflow:hidden}.joystick-container-1{position:relative;width:70vmin;height:70vmin}.joystick-base-1{position:absolute;width:100%;height:100%;background:#222;border-radius:50%;opacity:.6}.joystick-handle-1{position:absolute;top:50%;left:50%;width:80%;height:80%;background:#aaa;border-radius:50%;transform:translate(-50%,-50%);touch-action:none;transition:transform .1s ease}.joystick-container-2{position:relative;width:70vmin;height:50vmin}.joystick-base-2{position:absolute;width:100%;height:100%;background:#222;border-radius:10px;opacity:.6}.joystick-handle-2{position:absolute;top:50%;left:50%;width:80%;height:80%;background:#aaa;border-radius:10px;transform:translate(-50%,-50%);touch-action:none;transition:transform .1s ease}</style><div class=navbar><a onclick='showSection("control")'>CONTROL</a> <a onclick='showSection("settings")'>SETTINGS</a> <a onclick='showSection("info")'style=float:right>INFO</a></div><div class=redbar><p class=dotdot>Waiting for server</div><div class=greenbar><p>Connected!</div><div id=section-control><h1 class=block id=control-title style=padding-top:45px>CONTROL</h1><div class=white-box><div class=joystick-container-1 id="drive container"style=margin-left:auto;margin-right:auto><div class=joystick-base-1></div><div class=joystick-handle-1 id="drive handle"></div></div></div><hr><div class=white-box><div class=joystick-container-2 id="turret container"style=margin-left:auto;margin-right:auto><div class=joystick-base-2></div><div class=joystick-handle-2 id="turret handle"></div></div></div><hr></div><div class=hide id=section-settings><h1 class=block id=settings-title style=padding-top:45px>SETTINGS</h1><div class=cred-container><h2>AP mode:</h2><label>Network Name: <small id=ssid_AP_warning>---</small></label> <input id=ssid_AP value0=""> <label>Password: <small id=pass_AP_warning>---</small></label> <input id=pass_AP value0=""type=password><div class=ip-box id=ip_AP>xxx.xxx.xxx.xxx</div><p class=button id=ap_restart onclick='blink(this),socket.send(this.id+";")'style=width:60vw>Restart AP WiFi</div><div class=cred-container><h2>STA mode:</h2><label>Network Name: <small id=ssid_STA_warning>---</small></label> <input id=ssid_STA value0=""> <label>Password: <small id=pass_STA_warning>---</small></label> <input id=pass_STA value0=""type=password><div class=ip-box id=ip_STA>xxx.xxx.xxx.xxx</div><p class=button id=sta_restart onclick='blink(this),socket.send(this.id+";")'style=width:60vw>Restart STA WiFi</div><div style=height:30px></div><div class=white-box><h2>Calibration:</h2><label>Drive motor deadzone</label><div id=deadzone></div><label>Traverse centre (PWM)</label><div id=traverse_centre></div><label>Traverse limit (PWM)</label><div id=traverse_limit></div><label>Elevation max (PWM)</label><div id=elevation_max></div><label>Elevation min (PWM)</label><div id=elevation_min></div></div><div class=block><div class=button id=reset style=width:60vw onclick=confirmReset(this)>Factory Reset</div></div><hr><div style=height:50px></div></div><div class=hide id=section-info><h1 class=block id=info-title style=padding-top:45px>VERSION</h1><div class=white-box><p id=ver>---<hr></div><hr></div><script>let socket;
    let retryDelay = 1000;         // 1 s
    const maxRetryDelay = 10000;   // cap 10 s

    function blink(e) {
      e.disabled = true;
      e.classList.add("blink");
      setTimeout(() => {
        e.classList.remove('blink');
        e.disabled = false;
      }, 1000);
    }
    function clamp(number, min, max) {
      return number > max ? max : number < min ? min : number;
    }
    function triggerInputEvent(e) {
      const event = new Event('input', {
        bubbles: true,
        cancelable: true,
      });
      e.dispatchEvent(event);
    }

    function disableScroll()
    {
      window.addEventListener('touchmove', preventScroll, { passive: false });
    }
    function enableScroll()
    {
      window.removeEventListener('touchmove', preventScroll, { passive: false });
    }
    function preventScroll(e)
    {
      e.preventDefault();
    }

    function createSlider(id, initial, min, max) {
      const parent = document.getElementById(id);
      parent.id = id + "-block";

      const slider = document.createElement('input');
      slider.className = 'k-slider';
      slider.id = id;
      slider.min = min;
      slider.max = max;
      
      slider.value = initial;
      parent.appendChild(slider);

      let active = false;
      let velocity = 0;
      let lastX = 0;
      let currentValue = initial;

      slider.addEventListener('mousedown', startDrag);
      slider.addEventListener('touchstart', startDrag);
      slider.addEventListener('input', () => {
        currentValue = slider.value;
      });

      function startDrag(e) {
        active = true;
        velocity = 0;
        lastX = e.clientX || e.touches[0].clientX;
        disableScroll();
      }

      function drag(e) {
        if (!active) return;
        const x = e.clientX || e.touches[0].clientX;
        velocity = x - lastX;
        lastX = x;
        animate();
      }

      function stopDrag() {
        if (active)
        {
          active = false;
          enableScroll();
          animate(true); // final send
        }
      }

      function animate(send = false) {
        if (!active && Math.abs(velocity) < 0.2) {
          if (send) {
            socket.send(`settings;${id}=${slider.value}`);
            //console.log(`settings;${id}=${slider.value}`);
            blink(slider);
          }
          return;
        }
        currentValue = clamp(Number(currentValue) + velocity * 0.03, min, max);
        velocity *= active ? 0.80 : 0.99;
        slider.value = Math.round(currentValue);
        requestAnimationFrame(() => animate(send));
      }

      document.addEventListener('mousemove', drag);
      document.addEventListener('touchmove', drag);
      document.addEventListener('mouseup', stopDrag);
      document.addEventListener('touchend', stopDrag);
    }

    function showSection(section) {
      let sections = ["control", "settings", "info"];
      sections.forEach(id => {
        const el = document.getElementById("section-" + id);
        if (el) el.classList.add("hide");
      });
      const target = document.getElementById("section-" + section);
      if (target) {
        target.classList.remove("hide");
        if (section == sections[0]) {
          socket.send("state;");
          console.log("ask for control state");
        }
        else if (section == sections[1]) {
          socket.send("settings;");
          console.log("ask for settings");
        }
        else if (section == sections[2]) {
          socket.send("info;");
          console.log("ask for info");
        }
      }


    }
    function confirmReset(e) {
      if (confirm("Are you sure you want to factory reset? The page will reload.")) {
        blink(e);
        socket.send('settings-reset;');
        location.reload();
      }
    }
    function parseSettings(rawSettings) {
      let settings = {};
      let parts = rawSettings.split(';');
      parts.forEach(part => {
        let [key, value] = part.split('=');
        if (key && value) {
          settings[key.trim()] = value.trim();
        }
      });
      return settings;
    }
    function updateSettingsUI(settings) {
      Object.entries(settings).forEach(([key, value]) => {
        const element = document.getElementById(key);
        if (element) {
          if (key === "pass_STA") {
            element.value = '*'.repeat(value);
          } else if (key === "ip_STA" || key === "ip_AP") {
            element.innerHTML = value;
          } else {
            element.value = value;
            triggerInputEvent(element)
          }
        }
      });
    }

    const redBar = document.querySelector('.redbar');
    const greenBar = document.querySelector('.greenbar');
    let heartbeatConnected = false;
    let lastMsgTimer = null;
    function heartbeat() {
      // clear old timer
      clearTimeout(lastMsgTimer);
      // show greenbar 
      if (!heartbeatConnected) {
        heartbeatConnected = true;
        redBar.classList.remove('visible');
        greenBar.classList.add('visible');
        setTimeout(() => greenBar.classList.remove('visible'), 1000);
      }
      // expire after 12s if no new heartbeat
      lastMsgTimer = setTimeout(() => {
        clearInterval(tickTimer);
        redBar.classList.add('visible');
        heartbeatConnected = false;
        socket.close(1000, "Heartbeat closure");
        console.warn("Heartbeat closure");
      }, 12000);
    }

    function connectWebSocket(ip) {
      socket = new WebSocket(`ws://${ip}/ws`);
      console.log(`ws://${ip}/ws`);

      socket.onopen = function () {
        retryDelay = 1000;
        console.log("WebSocket connected to:", ip);
        //socket.send("Hello ESP32!");
        socket.send("settings;");
        heartbeat();
      };

      socket.onmessage = function (event) {
        console.log("Message:", event.data);
        ws_parse(event.data);
        heartbeat();
      };

      socket.onclose = function () {
        redBar.classList.add('visible');
        console.warn(`WebSocket closed. Retry in ${retryDelay / 1000}s`);
        setTimeout(() => connectWebSocket(ip), retryDelay);
        retryDelay = Math.min(retryDelay * 2, maxRetryDelay); // exponential backoff
      };
      socket.onerror = (err) => {
        console.error("WebSocket error:", err);
        socket.close(); // triggers onclose to retry
      };
    }
    
    function ws_parse(raw) {
      const parts = raw.split(';');
      const tag = parts[0];

      switch (tag) {
        case "settings":
          console.log("recieve settings data");
          const settingsData = raw.substring(raw.indexOf(";") + 1); // Extract actual settings part
          const parsedSettings = parseSettings(settingsData);
          updateSettingsUI(parsedSettings);
          console.log(parsedSettings); // View parsed settings object
          break;
        case "info":
          console.log("recieve version data");
          const versionData = raw.substring(raw.indexOf(";") + 1); // Extract version data
          document.getElementById("ver").innerHTML = versionData;
          break;
        default:
          break;
      }
    }

    // ---------- IP Address Handling ----------
    let ip_AP = document.getElementById("ip_AP");
    let ip_STA = document.getElementById("ip_STA");
    /*SSID PASS*/
    function setupTextInputSanitizer(inputId, warningId, maxLength, allowEmpty, label) {
      const input = document.getElementById(inputId);
      const warning = document.getElementById(warningId);

      input.addEventListener("input", () => {
        // Block ';' and control characters
        input.value = input.value.replace(/[\x00-\x1F\x7F;]/g, '').slice(0, maxLength);

        let reason = "---";
        let valid = true;

        if (!allowEmpty && input.value.length === 0) {
          reason = "Cannot be empty";
          valid = false;
        } else if (input.value.length > maxLength) {
          reason = `Too long (max ${maxLength})`;
          valid = false;
        }
        if (label.startsWith("pass") && input.value.length > 0 && input.value.length < 8) {
          reason = "Password too short (min 8)";
          valid = false;
        }
        input.style.backgroundColor = valid ? "" : "#fcc";
        warning.innerHTML = `<small>${valid ? "Valid" : reason}</small>`;

        if (valid) {
          socket.send(`settings;${label}=${input.value}`);
          console.log(`settings;${label}=${input.value}`);
        }
      });
    }
    setupTextInputSanitizer("ssid_AP", "ssid_AP_warning", 22, false, "ssid_AP");// 15 characters for IP display
    setupTextInputSanitizer("pass_AP", "pass_AP_warning", 63, true, "pass_AP");
    setupTextInputSanitizer("ssid_STA", "ssid_STA_warning", 32, false, "ssid_STA");
    setupTextInputSanitizer("pass_STA", "pass_STA_warning", 63, true, "pass_STA");
    
    class Joystick
    {
      constructor(options)
      {
        this.container = options.container;
        this.handle = options.handle;
        this.maxDistanceX = options.maxDistanceX;
        this.maxDistanceY = options.maxDistanceY;
        this.maxDistance = Math.hypot(this.maxDistanceX, this.maxDistanceY);
        this.returnX = options.returnX ?? true;
        this.returnY = options.returnY ?? true;
        this.onChange = options.onChange;
        this.onEnd = options.onEnd;
        this.roundLimit = options.roundLimit ?? false;

        this.drag = false;
        this.touchId = null;       // ← added
        this.offset = { x: 0, y: 0 };
        this.output = { x: 0, y: 0 };

        this._bindEvents();
      }

      _bindEvents()
      {
        this.handle.addEventListener('touchstart', (e) => this._startTouch(e), { passive: false });
        this.handle.addEventListener('mousedown', (e) => this._startMouse(e));

        window.addEventListener('touchmove', (e) => this._moveTouch(e), { passive: false });
        window.addEventListener('mousemove', (e) => this._moveMouse(e));

        window.addEventListener('touchend', (e) => this._endTouch(e));
        window.addEventListener('mouseup', () => this._endMouse());
      }

      // --- TOUCH HANDLING --------------------------------------------------------

      _startTouch(e)
      {
        if (this.drag) return;     // already handling another finger
        const t = e.changedTouches[0];

        this.touchId = t.identifier;   // ← claim this finger
        this.drag = true;
        this.handle.style.transition = 'none';

        const rect = this.handle.getBoundingClientRect();
        this.offset.x = t.clientX - (rect.left + rect.width / 2);
        this.offset.y = t.clientY - (rect.top + rect.height / 2);

        this._setFromTouch(t);
      }

      _moveTouch(e)
      {
        if (!this.drag) return;

        // find the *correct* touch by ID
        const t = [...e.changedTouches].find(t => t.identifier === this.touchId);
        if (!t) return;

        this._setFromTouch(t);
      }

      _endTouch(e)
      {
        if (!this.drag) return;

        // only release when *our* finger lets go
        for (const t of e.changedTouches)
        {
          if (t.identifier === this.touchId)
          {
            this.touchId = null;
            this._finish();
            break;
          }
        }
      }

      _setFromTouch(t)
      {
        const rect = this.container.getBoundingClientRect();
        const px = t.clientX - rect.left - rect.width / 2;
        const py = t.clientY - rect.top - rect.height / 2;

        this._apply(px - this.offset.x, py - this.offset.y);
      }

      // --- MOUSE HANDLING (unchanged except simplified) ---------------------------

      _startMouse(e)
      {
        this.drag = true;
        this.handle.style.transition = 'none';

        const rect = this.handle.getBoundingClientRect();
        this.offset.x = e.clientX - (rect.left + rect.width / 2);
        this.offset.y = e.clientY - (rect.top + rect.height / 2);

        this._moveMouse(e);
      }

      _moveMouse(e)
      {
        if (!this.drag) return;

        const rect = this.container.getBoundingClientRect();
        const px = e.clientX - rect.left - rect.width / 2;
        const py = e.clientY - rect.top - rect.height / 2;

        this._apply(px - this.offset.x, py - this.offset.y);
      }

      _endMouse()
      {
        if (!this.drag) return;
        this._finish();
      }

      // --- CORE ------------------------------------------------------------------

      _apply(x, y)
      {
        if (this.roundLimit)
        {
          const dist = Math.hypot(x, y);
          if (dist > this.maxDistance)
          {
            const angle = Math.atan2(y, x);
            x = Math.cos(angle) * this.maxDistance;
            y = Math.sin(angle) * this.maxDistance;
          }
        }
        else
        {
          x = clamp(x, -this.maxDistanceX, this.maxDistanceX);
          y = clamp(y, -this.maxDistanceY, this.maxDistanceY);
        }

        this.handle.style.transform =
          `translate(calc(-50% + ${x}px), calc(-50% + ${y}px))`;

        this.output.x = +(x / this.maxDistanceX).toFixed(3);
        this.output.y = +(-y / this.maxDistanceY).toFixed(3);

        if (this.onChange) this.onChange(this.output);
      }

      _finish()
      {
        this.drag = false;
        this.handle.style.transition = 'transform 0.2s ease';

        const x = this.returnX ? 0 : this.output.x * this.maxDistanceX;
        const y = this.returnY ? 0 : -this.output.y * this.maxDistanceY;

        this._apply(x, y);

        if (this.returnX && this.returnY)
          this.handle.style.transform = 'translate(-50%, -50%)';

        if (this.onEnd) this.onEnd(this.output);
      }
    }



    function mapJoystickToWheels(x, y)
    {
      x = Math.max(-1, Math.min(1, x));
      y = Math.max(-1, Math.min(1, y));
      let left = y + x;
      let right = y - x;
      const maxVal = Math.max(Math.abs(left), Math.abs(right));
      if (maxVal > 1)
      {
        left /= maxVal;
        right /= maxVal;
      }
      const L = Math.round(left * 128);
      const R = Math.round(right * 128);

      return { L, R };
    }

    const driveJoy = new Joystick({
      container: document.getElementById('drive container'),
      handle: document.getElementById('drive handle'),
      maxDistanceX: 150,
      maxDistanceY: 150,
      roundLimit: true,
      returnX: true,
      returnY: true,

      onChange: (out) =>
      {
        const wheels = mapJoystickToWheels(out.x, out.y);
        socket.send(`drv;${wheels.L},${wheels.R}`);
        //console.log(`drv;${wheels.L},${wheels.R}`);
      }
    });

    const turretJoy = new Joystick({
      container: document.getElementById('turret container'),
      handle: document.getElementById('turret handle'),
      maxDistanceX: 150,
      maxDistanceY: 80,
      returnX: true,   // stays centred horizontally
      returnY: false,  // vertical position is held

      onChange: (out) =>
      {
        const pitch = Math.round(out.y * 128);   // -128..128
        const yaw = Math.round(out.x * 128);

        socket.send(`tur;${yaw},${pitch}`);
        //console.log(`tur;${yaw},${pitch}`);
      }
    });

    connectWebSocket(location.host);

    let tickTimer_t = null;
    window.addEventListener("load", (event) => {
      // slider creation
      createSlider("deadzone", 0, 1, 1023);
      createSlider("traverse_limit", 100, -1000, 1000);
      createSlider("traverse_centre", 1500, 1000, 2000);
      createSlider("elevation_max", 2200, 500, 2500);
      createSlider("elevation_min", 1300, 500, 2500);

      ws_parse(test_settings_data);

      showSection("control");
    });</script>)=====";