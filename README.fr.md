[README.en.md](README.en.md)

parce que[ArduinoSTL d'origine](https://github.com/mike-matera/ArduinoSTL)L'auteur ne l'a pas mis à jour depuis longtemps (202203~202212).J'ai publié cette branche dans la bibliothèque publique Arduino et l'ai renommée Cpp_Standard_Library pour la distinguer.

Cette bibliothèque tente d'implémenter la plupart des fonctions de la bibliothèque standard C++ 11~17 (STL) sur Arduino. En plus d'ArduinoSTL, certaines fonctions de cette bibliothèque font également référence à MSVC, LLVM et boost. L'interface étant basée sur STL, il n'est pas nécessaire d'écrire des documents supplémentaires. Vous pouvez vous référer à n'importe quel document STL faisant autorité. Sauf indication contraire, l'utilisation de cette bibliothèque doit être la même. Si elle est différente, cela devrait être un bug. Vous êtes invités à soumettre un problème.

Si vous avez besoin de certaines fonctions qui se trouvent dans la bibliothèque standard mais qui ne sont pas fournies dans cette bibliothèque, vous pouvez également soumettre un problème, et l'auteur le implémentera d'abord pour vous.

Prend actuellement en charge les architectures AVR et SAM. Il n'est pas prévu de prendre en charge ESP32 pour le moment, car ESP32 a officiellement fourni une bibliothèque standard, et l'utilisation simultanée de deux ensembles de bibliothèques standard entraînera de nombreux problèmes difficiles à résoudre.

Avant d'inclure un fichier d'en-tête standard C++, vous devez d'abord inclure`Cpp_Standard_Library.h`. Il s'agit d'une invite adressée à l'IDE Arduino, indiquant au compilateur que cette bibliothèque doit être incluse dans le processus de compilation.

# Fonctions de signature (sans s'y limiter)

-   `<algorithm> fill_n shuffle`
-   `<chrono> chrono::duration`
-   `<functional> std::function`
-   `<iostream> cin cout`Utiliser le port série comme flux d'entrée et de sortie standard
-   `<map>`
-   `<memory> unique_ptr`
-   `<random> mt19937, ArduinoUrng`：`mt19937`Il occupe beaucoup de mémoire (environ 5 Ko), utilisez-le donc avec prudence.`ArduinoUrng`est-ce spécifique à la plateforme Arduino`UniformRandomNumberGenerator`,Peut être utilisé comme`shuffle`。
-   `<ratio>`
-   `<set>`
-   `<type_traits>`
-   `<vector>`
-   `<xutility> std::begin std::end`

# README original (à titre de référence uniquement, certains contenus sont obsolètes)

Il s'agit d'une implémentation d'une bibliothèque standard C++ présentée sous forme de bibliothèque Arduino. La bibliothèque prend en charge l'enseignement de ma classe CS-11M en ajoutant des fonctionnalités clés C++ dans l'environnement Arduino.

La bibliothèque est portée depuis uClibc++ :

<http://git.uclibc.org/uClibc++>

Avec une implémentation de flux de la bibliothèque Arduino d'Andy Brown :

<http://andybrown.me.uk/2011/01/15/the-standard-template-library-stl-for-avr-with-c-streams/>

## Utilisation de printf() et scanf()

Le fichier d'en-tête ArduinoSTL contient du code pour lier un port série de votre choix à
les primitives stdio. Cela se produit automatiquement mais l'utilisateur doit quand même appeler
Série.begin()

```c++
#include <ArduinoSTL.h>

void setup() {
  Serial.begin(9600); 
  printf("Hello World\n");
}
```

## En utilisant`cin`un`cout`

Lorsque vous incluez ce fichier d'en-tête, vous obtenez automatiquement cin et cout en fonction de`Serial`. Voir ci-dessous pour savoir comment spécifier votre propre appareil. Voici un exemple de croquis utilisant`cin`et`cout`.

```c++
#include <ArduinoSTL.h>

using namespace std;

void setup() {
  Serial.begin(9600);
  cout << "Feed me an integers." << endl;
}

void loop() {
  int foo;
  if (cin >> foo) { 
    cout << "You fed me " << foo << endl;
  }else{
    cin.clear();
    cin.ignore();
  }
}
```

## Changer le port série

Vous pouvez changer le port série utilisé`cin`,`cout`et`printf()`utiliser. Vous pouvez utiliser des ports série intégrés (par ex.`Serial1`sur Leonardo) ou vous pouvez utiliser des ports série logiciels qui implémentent`Stream`.

### Utilisation d'un port intégré

Dans`src/ArduinoSTL.cpp`changer la valeur de`ARDUINOSTL_DEFAULT_SERIAL`. Laissez les autres valeurs par défaut sans commentaire.

### Utilisation d'un port SoftwareSerial.

Ensemble`ARDUINO_DEFAULT_SERAL`à`NULL`. Commentez les autres valeurs par défaut.

Voici un exemple de croquis qui utilise SoftwareSerial :

```c++
#include <ArduinoSTL.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1);

namespace std { 
  ohserialstream cout(mySerial);
  ihserialstream cin(mySerial);
}

void setup() {
  mySerial.begin(9600);
  ArduinoSTL_Serial.connect(mySerial);
}
```

## Éviter l'instanciation de`cin`et`cout`

Comment out`ARDUINOSTL_DEFAULT_CIN_COUT`et rien ne sera instancié. Vous devez commenter cet indicateur si vous avez l'intention de sélectionner un port série autre que celui par défaut. Il n'y a pas de frais généraux appréciables pour l'utilisation`printf()`vous ne pouvez donc pas actuellement éviter de l'initialiser.

## Problèmes connus

Impression de flotteurs et de doubles en utilisant`cout`ignore les spécificateurs de format.

uClibc semble être assez complet. Les chaînes et les vecteurs fonctionnent tous deux, même avec la quantité limitée de tas disponible pour Arduino. La page d'état d'uClibc++ peut être trouvée ici :

<https://cxx.uclibc.org/status.html>

Utilisez toujours le dernier IDE Arduino. Cette bibliothèque utilise la spécification de la bibliothèque Arduino IDE rev.2.1 avec des fonctionnalités disponibles uniquement sur Arduino 1.6.10 et versions ultérieures. Le cahier des charges peut être trouvé ici :

<https://github.com/arduino/Arduino/wiki/Arduino-IDE-1.5:-Library-specification>

## Licence

La bibliothèque uClibc++ est sous licence LGPL. Ce projet adopte la LGPL pour être compatible avec la majeure partie du code qu'il utilise. Sauf indication contraire, tout le code est sous licence LGPL. Il y a une exception :

-   src/serstream est sous licence BSD selon les souhaits d'Andy Brown ici :<http://andybrown.me.uk/terms-and-conditions/>