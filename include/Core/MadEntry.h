#pragma once


#define MAD_ENTRY(GameClass)		\
int main ()							\
{									\
	GameClass game;					\
	World::GetWorld()->StartGame();	\
	return 0;						\
}									\