const char config_html_str[] PROGMEM = "<head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1, minimum-scale=1\"> <style>@charset \"UTF-8\";body{font-family:system-ui,-apple-system,BlinkMacSystemFont,Segoe UI,Roboto,Oxygen,Ubuntu,Cantarell,Fira Sans,Droid Sans,Helvetica Neue,sans-serif;line-height:1.4;max-width:800px;margin:20px auto;padding:0 10px;color:#dbdbdb;background:#202b38;text-rendering:optimizeLegibility}button,input,textarea{transition:background-color .1s linear,border-color .1s linear,color .1s linear,box-shadow .1s linear,transform .1s ease}h1{font-size:2.2em;margin-top:0}h1,h2,h3,h4,h5,h6{margin-bottom:12px}h1,h2,h3,h4,h5,h6,strong{color:#fff}b,h1,h2,h3,h4,h5,h6,strong,th{font-weight:600}blockquote{border-left:4px solid rgba(0,150,191,.67);margin:1.5em 0;padding:.5em 1em;font-style:italic}blockquote>footer{margin-top:10px;font-style:normal}address,blockquote cite{font-style:normal}button,input[type=button],input[type=checkbox],input[type=submit]{cursor:pointer}input:not([type=checkbox]):not([type=radio]),select{display:block}button,input,select,textarea{color:#fff;background-color:#161f27;font-family:inherit;font-size:inherit;margin-right:6px;margin-bottom:6px;padding:10px;border:none;border-radius:6px;outline:none}button,input:not([type=checkbox]):not([type=radio]),select,textarea{-webkit-appearance:none}textarea{margin-right:0;width:100%;box-sizing:border-box;resize:vertical}button,input[type=button],input[type=submit]{padding-right:30px;padding-left:30px}button:hover,input[type=button]:hover,input[type=submit]:hover{background:#324759}button:focus,input:focus,select:focus,textarea:focus{box-shadow:0 0 0 2px rgba(0,150,191,.67)}button:active,input[type=button]:active,input[type=checkbox]:active,input[type=radio]:active,input[type=submit]:active{transform:translateY(2px)}button:disabled,input:disabled,select:disabled,textarea:disabled{cursor:not-allowed;opacity:.5}::-webkit-input-placeholder{color:#a9a9a9}:-ms-input-placeholder{color:#a9a9a9}::-ms-input-placeholder{color:#a9a9a9}::placeholder{color:#a9a9a9}a{text-decoration:none;color:#41adff}a:hover{text-decoration:underline}code,kbd{background:#161f27;color:#ffbe85;padding:5px;border-radius:6px}pre>code{padding:10px;display:block;overflow-x:auto}img{max-width:100%}hr{border:none;border-top:1px solid #dbdbdb}table{border-collapse:collapse;margin-bottom:10px;width:100%}td,th{padding:6px;text-align:left}th{border-bottom:1px solid #dbdbdb}tbody tr:nth-child(2n){background-color:#161f27}::-webkit-scrollbar{height:10px;width:10px}::-webkit-scrollbar-track{background:#161f27;border-radius:6px}::-webkit-scrollbar-thumb{background:#324759;border-radius:6px}::-webkit-scrollbar-thumb:hover{background:#415c73} </style> </head> <body> <h2>Set up Induti</h2> <form method=\"POST\"> <button type=\"button\" id=\"dialog-trigger\"> Select Available SSIDs </button> <br> <label for=\"ssid\">SSID</label> <input type=\"text\" name=\"ssid\" id=\"ssid\"> <label for=\"password\">Password</label> <input name=\"password\" id=\"password\" type=\"password\"> <input type=\"checkbox\" name=\"showPassword\" id=\"showPassword\"> <label for=\"showPassword\">Show Password</label> <br> <input type=\"submit\" value=\"Submit\"> </form> <dialog id=\"dialog\"> <header>Select SSID</header> <form method=\"dialog\"> <menu id=\"ssidList\"> </menu> </form> </dialog>  <script>var url=\"http://\" + location.hostname + (location.port ? ':' + location.port : '');fetch(url+\"/wifiList\").then((response)=>{return response.text();}).then((text)=>{var ssidList=document.getElementById(\"ssidList\"); var wifiList=text.split(\"|\"); wifiList.pop(); for(var i=0; i< wifiList.length; i++){var button=document.createElement(\"button\"); button.setAttribute(\"value\", wifiList[i]); button.innerText=wifiList[i]; ssidList.appendChild(button);}var button=document.createElement(\"button\"); button.setAttribute(\"value\", \"cancelWhyIsThisASSID\"); button.innerText=\"Cancel\"; ssidList.appendChild(button);});document.getElementById(\"showPassword\").addEventListener(\"change\", function(){document.getElementById(\"password\").setAttribute(\"type\",this.checked ? \"text\":\"password\");});document.getElementById(\"dialog-trigger\").addEventListener(\"click\", function(){document.getElementById(\"dialog\").showModal();});document.getElementById(\"dialog\").addEventListener(\"close\",function(e){if(e.target.returnValue !=\"cancelWhyIsThisASSID\") document.getElementById(\"ssid\").value=e.target.returnValue;});</script></body></html>";

//www.nousphere.net/cleanspecial.php Cleaning up html
//https://ajusa.github.io/lit/docs/lit.html?
//https://www.willpeavy.com/tools/minifier/