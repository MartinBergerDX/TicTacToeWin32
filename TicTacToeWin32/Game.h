#pragma once
#ifndef _GAME_H_

#include <windows.h>
#include <stdio.h>
#include "AI.h"


char buffer[64];

// DEFINES:

#define FIELDS    9
#define DIVISIONS 3
#define GAME_OVER 0x40000	// position 19

#define PLAYER_AI		0x80000 	// position 20, AI player

#define EMPTY		0x0		// binary 00
#define SET_X		0x1		// binary 01
#define SET_O		0x2		// binary 10

#define PLAYER1X	SET_X
#define PLAYER2O	SET_O



// STRUCTURE DECLARATIONS:

struct ticTacToe
{	// 32-bit space
   unsigned int f1		 : 2;
   unsigned int f2		 : 2;
   unsigned int f3		 : 2;
   unsigned int f4		 : 2;
   unsigned int f5		 : 2;
   unsigned int f6		 : 2;
   unsigned int f7		 : 2;
   unsigned int f8		 : 2;
   unsigned int f9		 : 2;
   unsigned int gameOver : 1;
   unsigned int player	 : 1;
   unsigned int empty	 : 12;
};

// use a union as a means of combining the structure approach with the bitwise approach.
// look at data as struct or as unsigned int:
union Views
{
   struct ticTacToe	st_view;
   unsigned int		ui_view;
};



// FUNCTION PROTOTYPES:

char* itobs (unsigned int n, char* ps);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
short checkWinState (HWND hwnd, HDC hdc, int cxBlock, int cyBlock, const union Views* game);
unsigned int binaryOffset (int x, int y);


#endif // _GAME_H_