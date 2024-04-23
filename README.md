# Wordle en C Client/Serveur

## Description
Ce projet a été réalisé lors de lors de ma 2ème année de licence informatique. 
Implémentation simple du jeu Wordle, écrit en C. Le jeu consiste à deviner un mot de cinq lettres en un nombre limité de tentatives. Après chaque tentative, des indices sont donnés pour aider le joueur à se rapprocher de la réponse correcte.
Le serveur gère les connexions entrantes des clients et enregistre les activités dans un journal. Les clients peuvent se connecter au serveur pour envoyer et recevoir des messages.

## Fonctionnalités
- Jeu de devinettes de mots basé sur la console.
- Réponse en temps réel à chaque tentative avec feedback visuel.

## Compilation et Exécution
Utilisez le `Makefile` fourni pour compiler et exécuter le jeu :
```bash
make wordle
```
```bash
./wordle
```
