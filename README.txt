/***********************************************************************************************/
*										KENSEI.io					  							*
*																							  	*
*						Rodrigo D Moreira  ||  Antonio Agusto D Sousa							*
/***********************************************************************************************/

/***********************************************************************************************/
*										Bibliotecas					 						 	*
*				  	 																			*
*									  > SOIL (-lSOIL)											*
*									  > GLEW (-lGLEW)											*
*									> freeglut (-lglut)											*
*				  						> OpenGl (-lGL)											*
*				  						> GLU (-lGLU)											*
*				 				 		> Math.h (-lm)											*
*				  						> SDL2 (-lSDL2)											*
*								> SDL2_Mixer (-lSDL2_mixer)										*
*				  	 					> iostream 												*
*				 	 					> fstream												*
*				  	 					> string												*
*				 					 > bits/stdc++.h											*
/***********************************************************************************************/

/***********************************************************************************************/
*										Compilação											  	*
*												  												*
*	g++ -std=c++11 *.cpp -lglut -lGLU -lGL -lGLEW -lSOIL -lm -lSDL2_mixer -lSDL2 -g && ./a.out 	*
/***********************************************************************************************/

/***********************************************************************************************/
										Adicionais												

# Personagem
	- Perda de massa > espada diminui quando acerta-se um youkai (demônio)
	- Itens
		@ Kitsune (pequena raposa) dobra a velocidade do samurai
		@ Miko (sacerdotisa) gera uma barreira protetora no jogador
		@ Orbe aumenta tamanho da espada
	- Personagem morre quando é tocado por um youkai

# Textura
	- Texturas em todos os objetos
	- Texturas animadas para jogador e itens
	- Texturas originais todas desenhas por Rodrigo D Moreira
	- Fundo lindão (não sei ao certo oq vc considera isso) (mas ta lindão)

# Jogabilidade
	- Inteligência artificial (simples, mas interessante o suficiente para desafiar o jogador)
	- Controle via teclado do personagem e espada simultaneamente (espada)(giro de 45 em 45 graus)
	- Controle via teclado do personagem e espada via mouse (espada)(giro de 360º)
	- Nivel da espada > nivel define o tamanho da espada e a segurança para matar os inimigos

# HUD
	- Cenário maior que a câmera
	- Pontuação e níve da espada no canto da tela
	- Moldura para contexto do jogo
	- Portrait animado do personagem no canto da tela

# Musica
	- Música de fundo (Koto - CloZee)
	- Efeitos sonoros no menu e em coletáveis e buffs
		- Fatal Strike - N Audioman
		- SFX Powerup 47 - Jalastram
		- SFX Powerup 27 - Jalastram
		- Shinning 8 bit - MrTheNoronha
		- Jiraiya theme - Naruto ou Ninja Jiraiya 
				(não sei ao certo, procurava na realidade por "kabuki scream" e encontrei este)
		- For Honor War Start - Extraído de vídeo do jogo

# Menus
	- SplashScreen
	- Menu Principal
	- Menu de HighScore
	- Menu de Ajuda
	- Menu de Crédito

# Outros
	- Salvamento de HighScore em arquivo
/***********************************************************************************************/