#include "Game.h"



// GLOBALS:

union Views game;


// FUNCTION DEFINITIONS:

short checkWinState(HWND hwnd, HDC hdc, int cxBlock, int cyBlock, const union Views* game)
{
   HPEN hPen = CreatePen (PS_SOLID, 6, RGB(255, 0, 0)); 
   HGDIOBJ hgdi = SelectObject (hdc, hPen);
   unsigned int offset = 0;
   static TCHAR szBuffer[sizeof(unsigned int)*8] = TEXT ("0101100101101");
   //TextOut(hdc, 10, 10, szBuffer, lstrlen(szBuffer));

   unsigned int i = 0;

   // rows:
   for(i = 0; i < DIVISIONS; i++)
   {
      if(((game->ui_view >> (i*6)) | EMPTY) == EMPTY) 
         continue;

      //all horizontal passes for player X
      if((((game->ui_view >> (i*6))    & PLAYER1X) == PLAYER1X) &&
         (((game->ui_view >> (i*6+2))	& PLAYER1X) == PLAYER1X) &&
         (((game->ui_view >> (i*6+4))  & PLAYER1X) == PLAYER1X))
      {
         MoveToEx (hdc, 0,				 i * cyBlock + (cyBlock / 2), NULL);
         LineTo	(hdc, 3 * cxBlock, i * cyBlock + (cyBlock / 2));
         return PLAYER1X;
      }

      //all horizontal passes for player O
      if((((game->ui_view >> (i*6))	   & PLAYER2O) == PLAYER2O) &&
         (((game->ui_view >> (i*6+2))	& PLAYER2O) == PLAYER2O) &&
         (((game->ui_view >> (i*6+4))  & PLAYER2O) == PLAYER2O))
      {
         MoveToEx (hdc, 0,				 i * cyBlock + (cyBlock / 2), NULL);
         LineTo	(hdc, 3 * cxBlock, i * cyBlock + (cyBlock / 2));
         return PLAYER2O;
      }
   }

   // columns:
   for(i = 0; i < DIVISIONS; i++)
   {
      if(((game->ui_view >> (i*2)) | EMPTY) == EMPTY) continue;

      //all vertical passes for player X
      if((((game->ui_view >> (i*2))	   & PLAYER1X) == PLAYER1X) &&
         (((game->ui_view >> (i*2+6))	& PLAYER1X) == PLAYER1X) &&
         (((game->ui_view >> (i*2+12)) & PLAYER1X) == PLAYER1X))
      {
         MoveToEx (hdc, i * cxBlock + (cxBlock / 2), 0, NULL);
         LineTo	(hdc, i * cxBlock + (cxBlock / 2), 3 * cyBlock);
         return PLAYER1X;
      }

      //all vertical passes for player O
      if((((game->ui_view >> (i*2))	   & PLAYER2O) == PLAYER2O) &&
         (((game->ui_view >> (i*2+6))	& PLAYER2O) == PLAYER2O) &&
         (((game->ui_view >> (i*2+12)) & PLAYER2O) == PLAYER2O))
      {
         MoveToEx (hdc, i * cxBlock + (cxBlock / 2), 0, NULL);
         LineTo	(hdc, i * cxBlock + (cxBlock / 2), 3 * cyBlock);
         return PLAYER2O;
      }
   }

   //cross pass, upper left - lower right, player X
   if(( (game->ui_view 		   & PLAYER1X) == PLAYER1X) &&
      (((game->ui_view >> 8)	& PLAYER1X) == PLAYER1X) &&
      (((game->ui_view >> 16) & PLAYER1X) == PLAYER1X))
   {
      MoveToEx (hdc, 0,				 0, NULL);
      LineTo	(hdc, 3 * cxBlock, 3 * cyBlock);
      return PLAYER1X;
   }

   //cross pass, lower left - upper right, player X
   if((((game->ui_view >> 4)	& PLAYER1X) == PLAYER1X) &&
      (((game->ui_view >> 8)	& PLAYER1X) == PLAYER1X) &&
      (((game->ui_view >> 12) & PLAYER1X) == PLAYER1X))
   {
      MoveToEx (hdc, 0, 3 * cyBlock, NULL);
      LineTo	(hdc, 3 * cxBlock, 0);
      return PLAYER1X;
   }

   //cross pass, upper left - lower right, player O
   if(( (game->ui_view 		   & PLAYER2O) == PLAYER2O) &&
      (((game->ui_view >> 8)	& PLAYER2O) == PLAYER2O) &&
      (((game->ui_view >> 16) & PLAYER2O) == PLAYER2O))
   {
      MoveToEx (hdc, 0,				 0, NULL);
      LineTo	(hdc, 3 * cxBlock, 3 * cyBlock);
      return PLAYER2O;
   }

   //cross pass, lower left - upper right, player O
   if((((game->ui_view >> 4)	& PLAYER2O) == PLAYER2O) &&
      (((game->ui_view >> 8)	& PLAYER2O) == PLAYER2O) &&
      (((game->ui_view >> 12) & PLAYER2O) == PLAYER2O))
   {
      MoveToEx (hdc, 0, 3 * cyBlock, NULL);
      LineTo	(hdc, 3 * cxBlock, 0);
      return PLAYER2O;
   }

   // delete pen:
   DeleteObject (hgdi);

   return 0;
}


// determine field based on x,y coordinates.
// result is used to shift bits in ticTacToe data field.
unsigned int binaryOffset(int x, int y)
{
   return (y + (y * 2 + 1) + x) * 2 - 2;
}


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
   static TCHAR szAppName[] = TEXT ("TicTacToe") ;
   HWND         hwnd ;
   MSG          msg ;
   WNDCLASS     wndclass ;

   wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
   wndclass.lpfnWndProc   = WndProc ;
   wndclass.cbClsExtra    = 0 ;
   wndclass.cbWndExtra    = sizeof(long) ;
   wndclass.hInstance     = hInstance ;
   wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
   wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
   wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
   wndclass.lpszMenuName  = NULL ;
   wndclass.lpszClassName = szAppName ;

   if (!RegisterClass (&wndclass))
   {
      MessageBox (NULL, TEXT ("Program requires Windows NT!"), szAppName, MB_ICONERROR) ;
      return 0 ;
   }
   hwnd = CreateWindow (szAppName, TEXT ("Power Game"),
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      210, 210,
      NULL, NULL, hInstance, NULL) ;

   ShowWindow (hwnd, iCmdShow) ;
   UpdateWindow (hwnd) ;

   while (GetMessage (&msg, NULL, 0, 0))
   {
      TranslateMessage (&msg) ;
      DispatchMessage (&msg) ;
   }
   return msg.wParam ;
}


LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   static BOOL fState[DIVISIONS][DIVISIONS], win = FALSE;
   static int cxBlock, cyBlock, offset;
   unsigned int holder = 0;
   int x, y;
   HDC hdc;
   PAINTSTRUCT ps;
   //static union Views game;

   switch(message)
   {
   case WM_SIZE:
      {
         // block size:
         cxBlock = LOWORD(lParam) / DIVISIONS;
         cyBlock = HIWORD(lParam) / DIVISIONS;

         return 0;
      }

   case WM_SETFOCUS:
      ShowCursor(TRUE);
      return 0;

   case WM_KILLFOCUS:
      ShowCursor(FALSE);
      return 0;

   case WM_LBUTTONDOWN: //513
      {
         // determine which field is clicked:
         x = LOWORD(lParam) / cxBlock;    // 0, 1, 2
         y = HIWORD(lParam) / cyBlock;    // 0, 1, 2

         if(x < DIVISIONS && y < DIVISIONS)
         {
            // prevents continuing of a game if somebody did win:
            if(win == TRUE) 
               break; 

            // offset to shift bits to current field 
            // (x + (x * 2 + 1) + y) * 2 - 2;
            offset = binaryOffset(x, y); 

            // if field is not X or O, it is empty, so proceed:
            if((((game.ui_view >> offset) & PLAYER1X) != PLAYER1X) && (((game.ui_view >> offset) & PLAYER2O) != PLAYER2O)) 
            {
               // user turn:
               holder = game.ui_view;    // save whole game
               game.ui_view >>= offset;  // shift to current field
               game.ui_view |=  SET_X;   // set field to X value
               game.ui_view <<= offset;  // shift to normal position
               game.ui_view |=  holder;  // restore field

               // check for win state and draw line if so:
               hdc = GetDC (hwnd);
               if((checkWinState(hwnd, hdc, cxBlock, cyBlock, &game) != 0) && win == FALSE)
               {
                  win = TRUE;
                  InvalidateRect(hwnd, NULL, FALSE);
               }
               ReleaseDC (hwnd, hdc);

               // AI turn:
               // process AI here:
               // game union must be intact!
               processAI (&game); // modifies game union object

               // invalidate client area:
               InvalidateRect (hwnd, NULL, FALSE);
            }
         }
         return 0;
      }

   case WM_PAINT:
      {
         hdc = BeginPaint(hwnd, &ps);

         // drawd grid and symbols:
         for(x = 0; x < DIVISIONS; x++)
         {
            for(y = 0; y < DIVISIONS; y++)
            {
               Rectangle(hdc, x * cxBlock, y * cyBlock, (x + 1) * cxBlock, (y + 1) * cyBlock);
               offset = binaryOffset(x, y); // which field?

               // is empty?
               if(((game.ui_view >> offset) | EMPTY) == EMPTY) 
                  continue;

               // is X?
               if(((game.ui_view >> offset) & SET_X) == SET_X)
               {
                  MoveToEx	(hdc, x		* cxBlock, y	  * cyBlock, NULL);
                  LineTo	(hdc, (x+1) * cxBlock, (y+1) * cyBlock);
                  MoveToEx (hdc, x		* cxBlock, (y+1) * cyBlock, NULL);
                  LineTo	(hdc, (x+1) * cxBlock, y	  * cyBlock);
               }

               // is O?
               if(((game.ui_view >> offset) & SET_O) == SET_O)
               {
                  Ellipse(hdc, x * cxBlock, y * cyBlock, (x+1) * cxBlock, (y+1) * cyBlock);
               }
            }
         }

         // check for win state and draw line if so:
         if((checkWinState(hwnd, hdc, cxBlock, cyBlock, &game) != 0) && win == FALSE)
         {
            win = TRUE;
            InvalidateRect(hwnd, NULL, FALSE);
         }

         EndPaint(hwnd, &ps);
         return 0;
      }

   case WM_DESTROY:
      PostQuitMessage(0);
      return 0;

   } // end-case

   return DefWindowProc(hwnd, message, wParam, lParam);
}
