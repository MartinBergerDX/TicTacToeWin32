#include "AI.h"


/**
Explanation:

AI plays to block human player, unless it has a winning move like two positions with one empty.
The main goal of AI is to block human player.

AI first check if field is empty, in case AI is first on move.
If it is, it occupies position 5. That is done in function checkEmpty.

Next, if players was first player and has occupied position 5, AI randomly chooses his position.

Next are rows, column and cross checks.
Here, AI looks for a win combination for himself, and if there is one, he chooses that position.
If there is not, he will block player if player has such a combination.

If nothing of above is successful, human player has occupied any non-central (5th) position, 
so occupie a space opposite of him.

After all else fails, use any empty random position.
This probably means that only one position is left empty.

Enjoy :)
*/



// file scope statics:
static const unsigned int iEmpty = 0;
static const unsigned int iPlayer1_X = 1;
static const unsigned int iPlayer2_O = 2;

// file scope variables:
static int i = 0, j = 0, // counter
           tmp = 0; 
static unsigned int holder = 0;
static int offset = 0;



// BINARY FIELD:

/**
integer bit field:
00 00 00 00 00 00 0         0           00 00 00 00 00 00 00 00 00   // binary
                  player    game over   f9 f8 f7 f6 f5 f4 f3 f2 f1   // function
                                        16 14 12 10  8  6  4  2  0   // bit offset to reach
*/



// function only applies to iPlayer2_O
int checkEmpty(union Views** game)
{
   // check if game field is iEmpty, if so, occupy central field:
   // this is in case if i someday implement "first play" option.
   for (i = 0; i < FIELDS; i++)
   {
      if (((((*game)->ui_view >> (i * 2)) & iPlayer1_X) == iEmpty) ||
          ((((*game)->ui_view >> (i * 2)) & iPlayer2_O) == iEmpty))
      {
         break;
      }
      if (i == FIELDS - 1)
      {
         (((*game)->st_view).f5) = SET_O;
         return 1;
      }
   }

   return 0;
}


// function only applies to iPlayer2_O
int checkCentral (union Views** game)
{
   // if central field is X and others are iEmpty:
   tmp = 1;
   for (i = 0; i < 17; i += 2)
   {
      if (i == 8) // skip central position;
         continue;

      if ((((*game)->ui_view >> i) & iPlayer1_X) == iEmpty)
         continue;
      else
      {
         tmp = 0; // j is 1 if fields are iEmpty;
         break;
      }
   }
   if ((tmp == 1) && ((*game)->st_view.f5 == iPlayer1_X))
   {
      srand ((unsigned) time (NULL));
      while (offset == 8)
      {
         offset = ( rand () % 9 ) * 2;
      }
      holder = (*game)->ui_view;
      (*game)->ui_view =   (*game)->ui_view >> offset;
      (*game)->ui_view |=  SET_O;
      (*game)->ui_view =   (*game)->ui_view << offset;
      (*game)->ui_view |=  holder;
      return 1;
   }

   return 0;
}


int checkRows (union Views** game, unsigned int player)
{
   // check rows:
   // increment i is how much bits to shift to get to positions 1 (0), 4 (6), 7 (12)
   for (i = 0; i < 13; i += 6)
   {
      // XX[] or OO[]
      if (((((*game)->ui_view >> (i + 0)) & player) == player)   && // check is <player> is on position
          ((((*game)->ui_view >> (i + 2)) & player) == player)   && // check is <player> is on position
         // check if position is empty:
         (((((*game)->ui_view >> (i + 4)) & iPlayer1_X) == iEmpty) && ((((*game)->ui_view >> (i + 4)) & iPlayer2_O) == iEmpty)) )
      {
         holder = (*game)->ui_view;
         (*game)->ui_view =   (*game)->ui_view >> (i + 4);
         (*game)->ui_view |=  SET_O;
         (*game)->ui_view =   (*game)->ui_view << (i + 4);
         (*game)->ui_view |=  holder;
         return 1;
      }

      // X[]X or O[]O
      if (((((*game)->ui_view >> (i + 0)) & player) == player)  &&   // check is <player> is on position
         // check if position is empty:
         (((((*game)->ui_view >> (i + 2)) & iPlayer1_X) == iEmpty) && ((((*game)->ui_view >> (i + 2)) & iPlayer2_O) == iEmpty) ) && 
          ((((*game)->ui_view >> (i + 4)) & player) == player))      // check is <player> is on position

      {
         holder = (*game)->ui_view;
         (*game)->ui_view =   (*game)->ui_view >> (i + 2);
         (*game)->ui_view |=  SET_O;
         (*game)->ui_view =   (*game)->ui_view << (i + 2);
         (*game)->ui_view |=  holder;
         return 1;
      }

      // []XX or []OO
      if (( ((((*game)->ui_view >> (i + 0)) & iPlayer1_X) == iEmpty) && ((((*game)->ui_view >> (i + 0)) & iPlayer2_O) == iEmpty) ) && 
            ((((*game)->ui_view >> (i + 2)) & player) == player)   && 
            ((((*game)->ui_view >> (i + 4)) & player) == player))
      {
         holder = (*game)->ui_view;
         (*game)->ui_view =   (*game)->ui_view >> (i + 0);
         (*game)->ui_view |=  SET_O;
         (*game)->ui_view =   (*game)->ui_view << (i + 0);
         (*game)->ui_view |=  holder;
         return 1;
      }
   }

   return 0;
}


int checkColumns (union Views** game, unsigned int player)
{
   // check columns:
   // increment i is how much bits to shift to get to positions 1 (0), 2 (2), 3 (4)
   for (i = 0; i < 5; i += 2)
   {
      // up-down vertical: XX[] or OO[]
      if (((((*game)->ui_view >> (i + 0))  & player) == player) && 
          ((((*game)->ui_view >> (i + 6))  & player) == player) && 
         (((((*game)->ui_view >> (i + 12)) & iPlayer1_X) == iEmpty) && ((((*game)->ui_view >> (i + 12)) & iPlayer2_O) == iEmpty)) )

      {
         holder = (*game)->ui_view;
         (*game)->ui_view = (*game)->ui_view >> (i + 12);
         (*game)->ui_view |=  SET_O;
         (*game)->ui_view = (*game)->ui_view << (i + 12);
         (*game)->ui_view |=  holder;
         return 1;
      }

      // up-down vertical: X[]X or O[]O
      if (((((*game)->ui_view >> (i + 0))  & player) == player) && 
         (((((*game)->ui_view >> (i + 6))  & iPlayer1_X) == iEmpty) && ((((*game)->ui_view >> (i + 6)) & iPlayer2_O) == iEmpty) ) && 
          ((((*game)->ui_view >> (i + 12)) & player) == player))
      {
         holder = (*game)->ui_view;
         (*game)->ui_view = (*game)->ui_view >> (i + 6);
         (*game)->ui_view |=  SET_O;
         (*game)->ui_view = (*game)->ui_view << (i + 6);
         (*game)->ui_view |=  holder;
         return 1;
      }

      // up-down vertical: []XX or []OO
      if ((((((*game)->ui_view >> (i + 0))  & iPlayer1_X) == iEmpty) && ((((*game)->ui_view >> (i + 0)) & iPlayer2_O) == iEmpty) ) && 
           ((((*game)->ui_view >> (i + 6))  & player) == player) && 
           ((((*game)->ui_view >> (i + 12)) & player) == player))
      {
         holder = (*game)->ui_view;
         (*game)->ui_view = (*game)->ui_view >> (i + 0);
         (*game)->ui_view |=  SET_O;
         (*game)->ui_view = (*game)->ui_view << (i + 0);
         (*game)->ui_view |=  holder;
         return 1;
      }
   }

   return 0;
}


int checkCross (union Views** game, unsigned int player)
{
   // check cross:

   // upper-left to lower-right:
   if ((*game)->st_view.f1 == iEmpty && (*game)->st_view.f5 == player && (*game)->st_view.f9 == player) 
   { 
      (*game)->st_view.f1 = iPlayer2_O; 
      return 1; 
   }
   if ((*game)->st_view.f1 == player && (*game)->st_view.f5 == iEmpty && (*game)->st_view.f9 == player)
   { 
      (*game)->st_view.f5 = iPlayer2_O; 
      return 1; 
   }
   if ((*game)->st_view.f1 == player && (*game)->st_view.f5 == player && (*game)->st_view.f9 == iEmpty)
   { 
      (*game)->st_view.f9 = iPlayer2_O; 
      return 1; 
   }

   // upper-right to lower-left:
   if ((*game)->st_view.f3 == iEmpty && (*game)->st_view.f5 == player && (*game)->st_view.f7 == player)
   { 
      (*game)->st_view.f3 = iPlayer2_O; 
      return 1; 
   }
   if ((*game)->st_view.f3 == player && (*game)->st_view.f5 == iEmpty && (*game)->st_view.f7 == player)
   { 
      (*game)->st_view.f5 = iPlayer2_O; 
      return 1; 
   }
   if ((*game)->st_view.f3 == player && (*game)->st_view.f5 == player && (*game)->st_view.f7 == iEmpty)
   { 
      (*game)->st_view.f7 = iPlayer2_O; 
      return 1; 
   }

   return 0;
}


int checkOpposite (union Views** game)
{
   // check opposite:
   // if position is iPlayer1_X or iPlayer2_O and opposite position is iEmpty, occupy it with SET_O:
   for (i = 0; i < FIELDS; i++)
   {
      if (i == 4)    // skip center field.
         continue;

      // if field is already occupied by computer skip it:
      if ((((*game)->ui_view >> (i * 2)) & iPlayer2_O) == iPlayer2_O)
         continue;

      // offset for opposing field:
      offset = (FIELDS-1 - i) * 2;

      // check current field:
      if ((((*game)->ui_view >> (i * 2)) & iPlayer1_X) == iPlayer1_X)
      {
         // check opposite field:
         if ( ((((*game)->ui_view >> offset) & iPlayer1_X) == iEmpty) && ((((*game)->ui_view >> offset) & iPlayer2_O) == iEmpty) )
         {
            //offset = (FIELDS-1 - i) * 2;
            holder = (*game)->ui_view;
            (*game)->ui_view =   (*game)->ui_view >> offset;
            (*game)->ui_view |=  SET_O;
            (*game)->ui_view =   (*game)->ui_view << offset;
            (*game)->ui_view |=  holder;

            return 1;
         }
      }
   }

   return 0;
}


int checkRandom (union Views** game)
{
   // random:
   srand ((unsigned) time (NULL));
   // safety counter:
   i = 0;

   // assign to random position:
   while (1)
   {
      if (++i > 100)
         return 0; // error: catastrophic failure.

      offset = ( rand () % 9 ) * 2;
      if ( ((((*game)->ui_view >> offset) & iPlayer1_X) == iEmpty) && ((((*game)->ui_view >> offset) & iPlayer2_O) == iEmpty) )
      {
         holder = (*game)->ui_view;
         (*game)->ui_view =   (*game)->ui_view >> offset;
         (*game)->ui_view |=  SET_O;
         (*game)->ui_view =   (*game)->ui_view << offset;
         (*game)->ui_view |=  holder;
         break;
      }
   }
   return 1;

   return 0;
}


int processAI (union Views* game)
{
   if (checkEmpty (&game) == 1)
      return 1;

   if (checkCentral (&game) == 1)
      return 1;

   if (checkRows (&game, iPlayer2_O) == 1)
      return 1;
   else
      if (checkRows (&game, iPlayer1_X) == 1)
         return 1;

   if (checkColumns (&game, iPlayer2_O) == 1)
      return 1;
   else
      if (checkColumns (&game, iPlayer1_X) == 1)
         return 1;

   if (checkCross (&game, iPlayer2_O) == 1)
      return 1;
   else
      if (checkCross (&game, iPlayer1_X) == 1)
         return 1;

   if (checkOpposite (&game) == 1)
      return 1;

   if (checkRandom (&game) == 1)
      return 1;

   // error: AI failure.
   return 0;
}
