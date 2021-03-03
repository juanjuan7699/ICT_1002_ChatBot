# ICT_1002_ChatBot

![image](https://user-images.githubusercontent.com/71871190/109764444-83697b00-7c2e-11eb-8f1c-07187f4e570a.png)


## Objective
Chatbots – computer programs that can respond to queries typed in natural language – have recently
become a popular addition to web sites and other interactive applications.

The aim of this project is to implement a simple chatbot that can respond to simple queries made up of
a single question word (called the intent) and an object (called the entity). The chatbot will be able to answer questions such as Where is the printer? or What is C? by identifying the intent (where or what) and entity (printer or C) then looking up the answer corresponding to this pair in a simple database.

The chatbot will also be able to learn new answers by asking questions of the user. If a user asks a
question for which the database does not contain an answer, the user will be given the option to supply
an answer that can be used to respond to the same question in future. (This simulates having the chatbot
refer the unknown question to a third party who does know the answer.)

## Detailed Requirement
Your team is required to implement, in C, a chatbot of the kind described in the introduction. The
chatbot will converse in the console using a command-line-like interface; no graphical UI is required.

On each iteration of the main loop of the program (implemented for you in the main function in the
skeleton code), the program will display a prompt and wait for the user to type a question or instruction.
