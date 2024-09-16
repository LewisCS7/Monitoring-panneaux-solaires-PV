/*JavaScript pour le fonctionnement du plugin*/
document.getElementById("loginForm").addEventListener("submit", function(event) {
    event.preventDefault();
    
    var username = document.getElementById("username").value;
    var password = document.getElementById("password").value;
    
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4) {
            if (this.status == 200) {
                // Authentification réussie, rediriger vers la page principale
                window.location.replace("/");
            } else {
                alert("Identifiants incorrects. Veuillez réessayer.");
            }
        }
    };
    xhttp.open("POST", "/login", true);
    xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xhttp.send("username=" + encodeURIComponent(username) + "&password=" + encodeURIComponent(password));
});