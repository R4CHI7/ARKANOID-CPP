#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<dos.h>
#include<process.h>
#include<stdlib.h>
#include<time.h>

union REGS i,o ;
int xmax , ymax  ;
int key_code = 0 ;
const int xoff=50, yoff=40 ; // Area offset coordinates
int score=0 ;
int num_lives = 3 ;

enum power{
	none, L, S
};

int getkey()
{
	i.h.ah = 0 ;
	int86(22,&i,&o) ;
	return(o.h.ah) ;
}

class brick{
	public :
		float xl, xr ; // xleft and xright
		float yt, yb ; // ytop and ybottom
		int flag ;   // to check whether it has been hit; 1 = hit
		power p ;

		brick()
		{
			  flag = 0 ;
			  p=none ;
		}

		void setF()
		{
			  flag=0;
		}

		void set(float x,float y)
		{
			  xl = x ;
			  xr = x+50 ;
			  yt = y ;
			  yb = y+20 ;
		}

		void draw()
		{
			  if(flag==0)
			  {
				   setcolor(WHITE) ;
				   rectangle(xl,yt,xr,yb) ;
				   rectangle(xl+2,yt+2,xr-2,yb-2) ;
				   setfillstyle(1,6) ;
				   floodfill(xl+5,yt+5,WHITE) ;
				   if(p==L)
					outtextxy((xl+xr)/2,(yt+yb)/2,"L") ;
				   if(p==S)
					outtextxy((xl+xr)/2,(yt+yb)/2,"S") ;
			  }
			  else
			  {
				   setcolor(BLACK) ;
				   rectangle(xl,yt,xr,yb) ;
				   rectangle(xl+2,yt+2,xr-2,yb-2) ;
				   setfillstyle(1,BLACK) ;
				   floodfill(xl+5,yt+5,WHITE) ;
				   if(p==L)
					outtextxy((xl+xr)/2,(yt+yb)/2,"L") ;
				   if(p==S)
					outtextxy((xl+xr)/2,(yt+yb)/2,"S") ;
			  }
		 }
};

//Defining Game Variables
int ballx, bally, ballxm, ballym ;
int xdir, ydir;
int padxl, padyt, padxr, padyb, padx1m, padx2m ;
int lev1=0,f,comp,compF, flagL, flagS, padF, init_f ;
int count,c;
time_t startL, endL, startS, endS ;
brick b[50] ;
int insF=0;




int bricksLvl1=30;

void level_2();

void hit()
{
	for(int i=0;i<count;i++)
	{
		  if(ballx>=b[i].xl && ballx<=b[i].xr && bally==b[i].yt-5 && b[i].flag==0){
			b[i].flag = 1 ;
			b[i].draw() ;
			ydir*=-1 ;
			comp--;
			score+=10 ;
			if(b[i].p==L)
			{
				startL = time(NULL) ;
				padxl = padxl-15 ;
				padxr = padxr+15 ;
			}
			if(b[i].p==S)
			{
				startS = time(NULL) ;
				padxl = padxl+15 ;
				padxr = padxr-15 ;
			}

		  }
		  if(ballx>=b[i].xl && ballx<=b[i].xr && bally==b[i].yb+5 && b[i].flag==0){
			b[i].flag = 1 ;
			b[i].draw() ;
			ydir*=-1 ;
			comp--;
			score+=10 ;
			if(b[i].p==L)
			{
				startL = time(NULL) ;
				padxl = padxl-15 ;
				padxr = padxr+15 ;
			}
			if(b[i].p==S)
			{
				startS = time(NULL) ;
				padxl = padxl+15 ;
				padxr = padxr-15 ;
			}
		  }
		  if(bally>=b[i].yt && bally<=b[i].yb && (ballx==b[i].xl-5||ballx==b[i].xr+5) && b[i].flag==0){
			b[i].flag = 1 ;
			b[i].draw() ;
			xdir*=-1 ;
			comp--;
			score+=10 ;
			if(b[i].p==L)
			{
				startL = time(NULL) ;
				padxl = padxl-15 ;
				padxr = padxr+15 ;
			}
			if(b[i].p==S)
			{
				startS = time(NULL) ;
				padxl = padxl+15 ;
				padxr = padxr-15 ;
			}
		  }
	}
	endL=time(NULL) ;
	if(endL-startL==10 && flagL==0)
	{
		padxl = padxl+15 ;
		padxr = padxr-15 ;
		flagL=1 ;
	}
	endS=time(NULL) ;
	if(endS-startS==10 && flagS==0)
	{
		padxl = padxl-15 ;
		padxr = padxr+15 ;
		flagS=1 ;
	}
}
/*
void instructions()
{       int n=1,code=0;
	cleardevice();
	//Instructions
	do
	{
		setcolor(WHITE) ;
		rectangle(50,40,600,450) ;
		rectangle(45,35,605,455) ;
		setfillstyle(1,RED) ;
		floodfill(49,39,WHITE) ;
		settextstyle(5,0,3);
		setcolor(BROWN);
		outtextxy(250,100,"ARKANOID");
		setcolor(BLACK);
		settextstyle(0,0,1);
		for(int i=0;i<=n;i++)
			outtextxy(95+textwidth("Press Enter to continue ")+i+textwidth(".")*(i+1)+2,400,".");
		setcolor(LIGHTGRAY);
		settextstyle(0,0,1);
		outtextxy(100,180,"Instructions to play the game : ");
		outtextxy(105,210,"(1) Once the game is loaded use the arrow keys to set");
		outtextxy(105+textwidth("(4)"),225," the paddle to the desired position");
		outtextxy(105,242,"(2) Use the Up arrow key to launch the ball ");
		outtextxy(105,259,"(3) Hit as many bricks as you can using the minimum");
		outtextxy(105+textwidth(("(4)")),270," number of lives (3 lives are provided)");
		outtextxy(105,287,"(4) You get +10 for hitting a brick and -20 for losing a life");
		outtextxy(105,305,"(5) The game contains two kinds of \"Power Ups\" ");
		outtextxy(105+textwidth("(4) "),320,"Grab them to see what happens");
		outtextxy(200,360,"Best of Luck.!");
		outtextxy(95,400,"Press any Key to continue ");
		for(i=0;i<n;i++)
			outtextxy(95+textwidth("Press Enter to continue ")+i+textwidth(".")*(i+1)+2,400,".");
		n++;
		if(n>=5)
			n=1;
		code=getkey();
		delay(250);
	}while(code==0);
	insF=1;
	settextstyle(0,0,1);
	setcolor(WHITE);
} */
void intro()
{       int n=0;
	while(!kbhit())
	{
		setcolor(WHITE) ;
		rectangle(50,40,600,450) ;
		rectangle(45,35,605,455) ;
		setfillstyle(1,RED) ;
		floodfill(49,39,WHITE) ;
		setcolor(BLACK);
		settextstyle(0,0,1);
		for(int i=0;i<=5;i++)
			outtextxy(100+textwidth("Press Enter to continue ")+i+textwidth(".")*(i+1)+2,230,".");
		settextstyle(5,0,5);
		setcolor(BROWN);
		outtextxy(200,100,"ARKANOID");
		settextstyle(0,0,1);
		setcolor(LIGHTGRAY);
		outtextxy(300,300,"Project prepared By : ");
		outtextxy(300,330,"Akshay Katyal (00611502710)");
		outtextxy(300,350,"Rachit Gupta (02411502710)");
		settextstyle(0,0,1);
		outtextxy(100,230,"Press Enter to continue ");
		for(i=0;i<n;i++)
			outtextxy(100+textwidth("Press Enter to continue ")+i+textwidth(".")*(i+1)+2,230,".");
		n++;
		if(n>=5)
			n=0;
		delay(150);
	}
	cleardevice();
	settextstyle(0,0,1);
	setcolor(WHITE);
}

void initialize()	//Function to initialize values
{	xdir = +1 ;
	ydir = -1 ;
	c=0 ;
	f=0 ;
	compF=0 ;
	xmax = 600 - 50 ;
	ymax = 450 - 40 ;
	padxl = xoff + xmax/2 - 70 ;
	padyt = yoff + ymax - 40 ;
	padxr = xoff + xmax/2 + 70 ;
	padyb = yoff + ymax - 30 ;
	padx1m=padxl ;
	padx2m=padxr ;
	ballx = xoff + xmax/2 ;
	bally = padyt-5 ;
	ballxm=ballx ;
	ballym=bally ;
	flagL = 0 ;
	flagS = 0 ;
	padF= 0 ;
	init_f = 0 ;
}

//PADDLE INITIALISATION

void pad_init()
{
	do
	{
		setcolor(BLACK) ;
		ballxm = ballx ;
		ballym = bally ;
		setfillstyle(1,BLACK) ;
		fillellipse(ballxm,ballym,5,5) ;
		setcolor(BLACK) ;
		rectangle(padx1m,padyt,padx2m,padyb) ;
		setcolor(WHITE) ;
		rectangle(padxl,padyt,padxr,padyb) ;
		ballx = (padxl+padxr)/2 ;
		bally = padyt - 5 ;
		setcolor(RED) ;
		setfillstyle(1,RED) ;
		fillellipse(ballx,bally,5,5) ;
		while(!kbhit())
		{
			//DO NOTHING
		}
		while(kbhit()&&f==0)
		   key_code = getkey() ;
		padx1m=padxl ;
		padx2m=padxr ;
		if(key_code==77)
		{
		   if(padxr<xoff+xmax)
		   {
			padxl+=10 ;
			padxr+=10 ;
		   }
		}
		else if(key_code==75)
		{
		   if(padxl>xoff)
		   {
			padxl-=10 ;
			padxr-=10 ;
		   }
		}
		else if(key_code==72&&init_f==0)
		{
			if(c==0)
			{
			    if(!(bally>=padyt-5&&ballx>=padxl&&ballx<=padxr))
				c++ ;
			}
			init_f=1 ;
			setcolor(BLACK) ;
			setfillstyle(1,BLACK) ;
			fillellipse(ballx,bally,5,5) ;
		}
		setcolor(WHITE) ;
		rectangle(padxl,padyt,padxr,padyb) ;
		if(!kbhit())
		{
		   f=0 ;
		   key_code=0 ;
		}
	}while(init_f==0) ;
}

void game_over()
{ 	cleardevice();
	int n=0;
	char S[50];
	do
	{	setcolor(WHITE) ;
		rectangle(50,40,600,450) ;
		rectangle(45,35,605,455) ;
		setfillstyle(1,RED) ;
		floodfill(49,39,WHITE) ;
		settextstyle(5,0,3);
		setcolor(BROWN);
		outtextxy(250,100,"ARKANOID");
		setcolor(BLACK);
		settextstyle(0,0,1);
		for(int i=0;i<=5;i++)
			outtextxy(95+textwidth("Press Any Key to Quit ")+i+textwidth(".")*(i+1)+2,400,".");
		setcolor(LIGHTGRAY);
		settextstyle(0,0,1);
		outtextxy(200,200,"Alas.! Your Game is over.!! ");
		outtextxy(200,250,"Your final Score : ");
		itoa(score,S,10);
		outtextxy(200+textwidth("Your final Score : "),250,S);
		settextstyle(0,0,1);
		outtextxy(95,400,"Press Any key to Quit ");
		for(i=0;i<n;i++)
			outtextxy(95+textwidth("Press Any key to Quit ")+i+textwidth(".")*(i+1)+2,400,".");
		n++;
		if(n>=5)
			n=0;
		delay(200);
	}while(!kbhit());
	exit(1);
}

void level_over()
{
	cleardevice();
	int n=0;
	char S[50];
	do
	{	setcolor(WHITE) ;
		rectangle(50,40,600,450) ;
		rectangle(45,35,605,455) ;
		setfillstyle(1,RED) ;
		floodfill(49,39,WHITE) ;
		settextstyle(5,0,3);
		setcolor(BROWN);
		outtextxy(250,100,"ARKANOID");
		setcolor(BLACK);
		settextstyle(0,0,1);
		for(int i=0;i<=5;i++)
			outtextxy(95+textwidth("Press Any Key to Continue ")+i+textwidth(".")*(i+1)+2,400,".");
		setcolor(LIGHTGRAY);
		settextstyle(0,0,1);
		outtextxy(200,200,"Congratulations Level Complete.!! ");
		outtextxy(200,250,"Your Present Score : ");
		itoa(score,S,10);
		outtextxy(200+textwidth("Your final Score : "),250,S);
		settextstyle(0,0,1);
		outtextxy(95,400,"Press Any key to Continue ");
		for(i=0;i<n;i++)
			outtextxy(95+textwidth("Press Any key to Continue ")+i+textwidth(".")*(i+1)+2,400,".");
		n++;
		if(n>=5)
			n=0;
		delay(200);
	}while(!kbhit());
	exit(0);
}

void level_1()
{
	setcolor(WHITE) ;
	outtextxy(20,12,"Lives left: ");
	int w = textwidth("Lives left: ");
	for(int i=1;i<=6;i+=2)
	{
		setcolor(BLACK) ;
		rectangle(w+20*i,10,w+20+20*i,20) ;
		setfillstyle(1,BLACK) ;
		floodfill(w+20*i + 1,11,BLACK) ;
	}

	for(i=1;i<=2*num_lives;i+=2)
	{
		setcolor(GREEN) ;
		rectangle(20*i+w,10,w+20+20*i,20) ;
		setfillstyle(1,GREEN) ;
		floodfill(w+20*i + 1,11,GREEN) ;
	}

       //BRICKS
       count=0 ;
       for(int l=-5;l<5;l++)
       {
	   for(int m=0;m<3;m++)
	   {
		if(b[count].flag!=1)
		{
		     b[count].set(xoff+xmax/2+50*l,yoff+50+20*m) ;
		     b[count].draw() ;
		}
		count++ ;
	    }
	}
	b[15].p = L ;
	b[7].p = S ;

	if(compF==0)
	{
		comp=count ;
		compF=1;
	}

	if(padF==0)
	{
		pad_init() ;
		padF = 1 ;
	}
	//BALL

       if(c==0)
       {
	   if(!(bally>=padyt-5&&ballx>=padxl&&ballx<=padxr))
		    c++ ;
       }

       if((ballx==xoff+5||ballx==xoff+xmax-5) && c!=0)
	   xdir*=-1 ;

       if(bally==yoff+5||(bally==padyt-5&&ballx>=padxl-5&&ballx<=padxr+5&&c!=0))
	   ydir*=-1 ;

       if((bally==padyt-5 && (ballx==padxl || ballx==padxr)) && c!=0)
       {
	   xdir*=-1 ;
	   ydir=-1 ;
       }

       if(bally>=padyt && bally<=padyb && (ballx==padxl-5 || ballx==padxr+5))
       {
	   num_lives-- ;
	   score-=20 ;
	   if(num_lives==0)
		game_over() ; //GAME OVER
	   else
	   {
		padF = 0 ;
		init_f = 0 ;
		level_1() ;
	   }
       }
       if(bally>=yoff+ymax-5)
       {
	   num_lives-- ;
	   score-=20 ;
	   if(num_lives==0)
		game_over() ; //GAME OVER
	   else
	   {
		padF=0 ;
		init_f = 0 ;
		level_1() ;
	   }
       }

       ballxm=ballx ;
       ballym=bally ;
       ballx += 5*xdir ;
       bally += 5*ydir ;
       delay(30);

       //PADDLE MOVEMENT
       setcolor(BLACK) ;
       rectangle(padx1m,padyt,padx2m,padyb) ;
       setcolor(WHITE) ;
       rectangle(padxl,padyt,padxr,padyb) ;
       while(kbhit()&&f==0)
	   key_code = getkey() ;
       padx1m=padxl ;
       padx2m=padxr ;
       if(key_code==77)
       {
	   if(padxr<xoff+xmax)
	   {
		padxl+=10 ;
		padxr+=10 ;
	   }
       }
       else if(key_code==75)
       {
	   if(padxl>xoff)
	   {
		padxl-=10 ;
		padxr-=10 ;
	   }
       }
       if(!kbhit())
       {
	   f=0 ;
	   key_code=0 ;
       }

	//HITTING
	hit();

	if(comp==0)
	{
		lev1=1;          //lvl cmplete
	}
}


void main()
{

	int gdriver = DETECT, gmode ;
	initgraph(&gdriver,&gmode,"") ;

	initialize();

	intro();

	do
	{
		//PLAY AREA
		setcolor(WHITE) ;
		rectangle(50,40,600,450) ;
		rectangle(45,35,605,455) ;
		setfillstyle(1,RED) ;
		floodfill(49,39,WHITE) ;

		//BALL MOVEMENT
	       setcolor(BLACK) ;
	       setfillstyle(1,BLACK) ;
	       fillellipse(ballxm,ballym,5,5) ;
	       setcolor(RED) ;
	       setfillstyle(1,RED) ;
	       fillellipse(ballx,bally,5,5) ;

	       level_1();

	}while(lev1!=1) ;

	cleardevice();

	level_over();

	initialize();

	cleardevice();

	closegraph() ;
}