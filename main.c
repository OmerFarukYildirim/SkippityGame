#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//to row from row dan büyük mü küçük mü diye bak büyükse aþaðý doðru mesela from row 0 sa 2. rowu boþ mu diye kontrol edicek sonra 4. rowu
// to col from coldan büyük mü diye bak büyükse saða doðru +2 indisini boþ mu diye kontrol ederek bakacak
//onun dýþýnda +1 indislerinin letterleri eþit mi diye bakýcak
#define MAX_SIZE 100


typedef struct {
    char** board;
    int size;
    int currentPlayer;
    int scorePlayer1;
    int scorePlayer2;
    int *letters1;
    int *letters2;
} Game;

void initializeBoard(Game* game) {
    int i,j;
    game->board = (char**)malloc(game->size * sizeof(char*));
    for (i = 0; i < game->size; i++) {
        game->board[i] = (char*)malloc(game->size * sizeof(char));
    }

    // Randomly initialize pieces on the board
    srand(time(NULL));
    for (i = 0; i < game->size; i++) {
        for (j = 0; j < game->size; j++) {

            game->board[i][j] = 'A' + rand() % 5;
        }
    }

    do
    {
        i = rand() % game->size;
        j = rand() % game->size;
        game->board[i][j] = '0';
    }while((i==0 && j==0) || (i== (game->size-1) && j == (game->size-1))
           || (i==0 && j==(game->size-1)) || (j==0 && i==(game->size-1)));

    game->currentPlayer = 1; // Player 1 starts
    game->scorePlayer1 = 0;
    game->scorePlayer2 = 0;
}

void printBoard(Game* game) {
    printf("\n  ");
    int i,j;
    for (j = 0; j < game->size; j++) {
        printf(" %d  ", j);
    }
    printf("\n");

    for (i = 0; i < game->size; i++) {
        printf("%d ", i);
        for (j = 0; j < game->size; j++) {
            printf("[%c] ", game->board[i][j]);
        }
        printf("\n");
    }
    printf("1. Oyuncunun puani: %d\n",game->scorePlayer1);
    printf("2. Oyuncunun puani: %d\n",game->scorePlayer2);
}
int undo(Game* game, int oldX, int oldY, int x, int y){
        int flagUndo;
        printf("Hamlenizi geri almak icin 1'e devam etmek icin 0'a tiklayin: ");
        scanf("%d",&flagUndo);
        if(flagUndo){
            char tmp = game->board[x][y];
            game->board[x][y] = '0'; // Eski konumu boþaltma
            game->board[oldX][oldY] = tmp; // Yeni konuma taþý yerleþtirme
            return flagUndo;
        }
    return flagUndo;
}

void writeGameToFile(Game* game, char* filename) {
    FILE* file = fopen(filename, "w"); // Dosyayý yazma modunda aç
    int i,j;
    if (file != NULL) {
        // Matrisi dosyaya yaz
        for (i = 0; i < game->size; i++) {
            for (j = 0; j < game->size; j++) {
                fprintf(file, "%c ", game->board[i][j]); // Matris elemanlarýný dosyaya yaz
            }
            fprintf(file, "\n"); // Her satýrýn sonuna yeni satýr karakteri ekle
        }
        // Skorlarý ve current player'ý dosyaya yaz
        fprintf(file, "%d\n", game->scorePlayer1); // 1. oyuncunun skoru
        fprintf(file, "%d\n", game->scorePlayer2); // 2. oyuncunun skoru
        fprintf(file, "%d\n", game->currentPlayer); // Current player
        for(i=0;i<5;i++){
            fprintf(file, "%d\n", game->letters1[i]);
        }
        for(i=0;i<5;i++){
            fprintf(file, "%d\n", game->letters2[i]);
        }

        fclose(file); // Dosyayý kapat
        printf("Oyun durumu '%s' dosyasýna baþarýyla kaydedildi.\n", filename);
    } else {
        printf("Dosya açma hatasý!\n");
    }
}


void readMatrixFromFile(char* filename, Game* game) {
    FILE* file = fopen(filename, "r"); // Dosyayý okuma modunda aç
    int i = 0, j = 0;
    char c;
    if (file != NULL) {
        // Karakterleri tek tek oku ve matrise yerleþtir
        while ((c = fgetc(file)) != EOF && i < MAX_SIZE) {
            if (c != ' ') { // Boþluk karakterini atla
                game->board[i][j] = c;
                j++;
            }
            if (c == '\n') { // Yeni satýr baþlangýcýný kontrol et
                i++;
                j = 0;
            }
        }
        game->size = i-13;
        // Skorlarý oku
        game->scorePlayer1 = atoi(game->board[game->size]);
        game->scorePlayer2 = atoi(game->board[game->size + 1]);
        game->currentPlayer = atoi(game->board[game->size + 2]);
        for(i=0;i<5;i++){
            game->letters1[i] = atoi(game->board[game->size + 3 + i]);
        }

        for(i=0;i<5;i++){
            game->letters2[i] = atoi(game->board[game->size + 8 + i]);
        }


        fclose(file); // Dosyayý kapat
        printf("Matris ve skorlar '%s' dosyasindan basariyla okundu.\n", filename);
    } else {
        printf("Dosya açma hatasý!\n");
    }
}


void playTurn(Game* game, int x, int y, int newX, int newY) {

    if (game->board[x][y] == '0') {
        printf("Invalid move! Selected cell is empty.\n");
        return;
    }else{

            if (isValidMove(game, x, y, newX, newY) == 1) {
            moveLetter(game, x, y, newX, newY);
            }
            if (isValidCapture(game, x, y, newX, newY) == 1) {
            capturePiece(game, x, y, newX, newY);
            }


    }
}

// Taþýn tahtada hareket edebileceði kontrolü
int isValidMove(Game* game,int fromRow, int fromCol, int toRow, int toCol) {
    // Geçerli bir konum kontrolü
    if (fromRow < 0 || fromRow >= game->size || fromCol < 0 || fromCol >= game->size ||
        toRow < 0 || toRow >= game->size || toCol < 0 || toCol >= game->size) {
        return 0;
    }
    // Dikey veya yatay yönde tek kare hareket kontrolü
    if (fromRow != toRow && fromCol != toCol) {
        return 0;
    }
    // Hedef konumun boþ olup olmadýðýnýn kontrolü
    if(toRow>fromRow){
        if (game->board[fromRow+2][fromCol] != '0') {
        return 0;
        }
    }else if(toRow<fromRow){
        if (game->board[fromRow-2][fromCol] != '0') {
        return 0;
        }
    }else if(toCol>fromCol){
        if (game->board[fromRow][fromCol+2] != '0') {
        return 0;
        }
    }else if(toCol<fromCol){
        if (game->board[fromRow][fromCol-2] != '0') {
        return 0;
        }
    }
    return 1;

}

// Taþý yeme iþleminin geçerliliðini kontrol etme
int isValidCapture(Game* game, int fromRow, int fromCol, int toRow, int toCol) {

    if(toRow>fromRow){
        if (game->board[fromRow+1][fromCol] == game->board[fromRow][fromCol]) return 0;
    }else if(toRow<fromRow){
        if (game->board[fromRow-1][fromCol] == game->board[fromRow][fromCol]) return 0;
    }else if(toCol>fromCol){
        if (game->board[fromRow][fromCol+1] == game->board[fromRow][fromCol]) return 0;
    }else if(toCol<fromCol){
        if (game->board[fromRow][fromCol-1] == game->board[fromRow][fromCol]) return 0;
    }
     return 1;

}

void moveLetter(Game* game,int x, int y, int newX, int newY) {
    char tmp = game->board[x][y];
    game->board[x][y] = '0'; // Eski konumu boþaltma
    game->board[newX][newY] = tmp; // Yeni konuma taþý yerleþtirme
}

// Bir taþý yeme iþlemini gerçekleþtirme
void capturePiece(Game* game, int fromRow, int fromCol, int toRow, int toCol) {
    // Yemek istediðiniz taþýn konumunu belirleyin
    int capturedRow, capturedCol;
    if (toRow > fromRow) {
        capturedRow = fromRow + 1;
        capturedCol = fromCol;
    } else if (toRow < fromRow) {
        capturedRow = fromRow - 1;
        capturedCol = fromCol;
    } else if (toCol > fromCol) {
        capturedRow = fromRow;
        capturedCol = fromCol + 1;
    } else if (toCol < fromCol) {
        capturedRow = fromRow;
        capturedCol = fromCol - 1;
    } else {
        // Geçersiz hareket durumu
        printf("Hatalý hareket.\n");
        return;
    }

    if(undo(game,fromRow,fromCol,toRow,toCol)==1){ // hamleyi geri alma
        return;
    }else{ // skor hesaplamasi
        skorHesap(game,capturedRow,capturedCol);
        game->board[capturedRow][capturedCol] = '0';
    }

}
void skorHesap(Game* game, int capturedRow, int capturedCol){
    char letter;
    if(game->currentPlayer==1){
            letter = game->board[capturedRow][capturedCol];
        switch(letter)
        {
            case 'A' :
                game->letters1[0]++;
            break;
            case 'B' :
                game->letters1[1]++;
            break;
            case 'C' :
                game->letters1[2]++;
            break;
            case 'D' :
                game->letters1[3]++;
            break;
            case 'E' :
                game->letters1[4]++;
            break;
        }
            if(game->letters1[0]!=0 && game->letters1[0]==game->letters1[1] && game->letters1[0]== game->letters1[2]
                   && game->letters1[0]== game->letters1[3] && game->letters1[0]== game->letters1[4]){
                    game->scorePlayer1 += 5;
                    printf("birinci oyuncu puani artti!!");
                }

        }else{
            letter = game->board[capturedRow][capturedCol];
        switch(letter)
        {
            case 'A' :
                game->letters2[0]++;
            break;
            case 'B' :
                game->letters2[1]++;
            break;
            case 'C' :
                game->letters2[2]++;
            break;
            case 'D' :
                game->letters2[3]++;
            break;
            case 'E' :
                game->letters2[4]++;
            break;
        }
                if(game->letters2[0]!=0 && game->letters2[0]==game->letters2[1] && game->letters2[0]== game->letters2[2]
                   && game->letters2[0]== game->letters2[3] && game->letters2[0]== game->letters2[4]){
                    printf("birinci oyuncu puani artti!!");
                    game->scorePlayer2 += 5;
                }
        }
        int i;
        printf("\n1. oyuncunun topladigi harf sayisi: ");
        for(i=0;i<5;i++){
            printf("%d ",game->letters1[i]);
        }
        printf("\n2. oyuncunun topladigi harf sayisi: ");
        for(i=0;i<5;i++){
            printf("%d ",game->letters2[i]);
        }
        printf("\n");
}

int main() {
    Game game;
    int choose,player;
    game.letters1 = (int *)calloc(5 , sizeof(int));
    game.letters2 = (int *)calloc(5 , sizeof(int));
    printf("Yeni oyun baslatmak icin 1'e onceki oyuna devam etmek istiyorsanýz 2'ye tiklayin: ");
    scanf("%d",&choose);
    if(choose==1){
        printf("Please enter game size: ");
        scanf("%d",&game.size);
        initializeBoard(&game);
        player=0;
    }else if(choose==2){
        int i,j;
        game.board = (char**)malloc(MAX_SIZE * sizeof(char*)); // okunacak matrise bellek tahsisi
        for (i = 0; i < MAX_SIZE; i++) {
            game.board[i] = (char*)malloc(MAX_SIZE * sizeof(char));
        }
        char *filename; // Dosya adý için bellek tahsis edilmiþ dizi
        filename = (char *)malloc(50 * sizeof(char));
        printf("Dosyanýn adýný .txt uzantýsý ile girin(matrix.txt): ");
        scanf("%s", filename);
        readMatrixFromFile(filename,&game);
        player = game.currentPlayer-1;
    }


    int x, y, newX, newY;
    int finish=0;

    while (finish!=1) {
        printBoard(&game);
        game.currentPlayer = (player % 2)+1;
        printf("Player %d's turn. Enter coordinates (x y): ", game.currentPlayer);
        scanf("%d %d", &x, &y);
        printf("Enter new coordinates (newX newY): ");
        scanf("%d %d", &newX, &newY);
        playTurn(&game, x, y, newX, newY);
        printf("Oyunu sonlandirmak icin 1'e basin: ");
        scanf("%d",&finish);
        if(finish==1){
            char *filename; // Dosya adý için bellek tahsis edilmiþ dizi
            filename = (char *)malloc(50 * sizeof(char));
            printf("Dosyanýn adýný .txt uzantýsý ile girin(matrix.txt): ");
            scanf("%s", filename);
           writeGameToFile(&game,filename);
        }
        player++;
        sleep(1);system("CLS");

        // Check game end conditions, update scores, switch players, etc.
        // Implement undo/redo functionality, saving/loading game state, scoring rules, AI for computer mode, etc.
    }
    return 0;
}
