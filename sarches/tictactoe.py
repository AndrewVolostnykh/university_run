print("Anrij Volostnykh|KV-71");

field = ["_", "_", "_",
        "_", "_", "_",
        "_", "_", "_"]
        
gameGoing = True
player = "X"
winner = None



def display():
    print(field[0] + " | " + field[1] + " | " + field[2])
    print(field[3] + " | " + field[4] + " | " + field[5])
    print(field[6] + " | " + field[7] + " | " + field[8])

def game():
    global winner
    
    display()
    while gameGoing:
        getTurn(player)
        display()
        checkGameResult()
    if winner == "X" or winner == "O":
        print("Congratulation " + winner + ", you win")
    else: 
        print("This is a tie( ")

def getTurn(player):
    position = input("Choose number from 1-9\n")
    print("Choosed number: " + position)
    
    position = int(position) - 1
    
    if position > 9 or position <= -1:
        print("Incorrect input, try again: ")
        return -1
    if field[position] == "X" or field[position] == "O":
        print("This place already filed, try another: ")
        return -1

    field[position] = player
    changePlayer()
    return position
    
def checkGameResult():
    checkForWin()
    checkForTie()
    return

def checkForWin():
    global gameGoing
    if checkRows() or checkColumns() or checkDiagonals():
        gameGoing = False
        return

def checkForTie():
    global gameGoing
    if "_" not in field: 
        gameGoing = False
    return

def changePlayer():
    global player
    if player == "X":
        player = "O"
    else:
        player = "X"
    return



def checkRows():
    global winner
    first = field[0] == field[1] == field[2] != "_"
    second = field[3] == field[4] == field[5] != "_"
    third = field[6] == field[7] == field[8] != "_"
    
    if first:
        winner = field[0]
    elif second: 
        winner = field[3]
    elif third:
        winner = field[6]
    else:
        return False
    return True

def checkColumns():
    global winner
    first = field[0] == field[3] == field[6] != "_"
    second = field[1] == field[4] == field[7] != "_"
    third = field[2] == field[5] == field[8] != "_"
    
    if first:
        winner = field[0]
    elif second: 
        winner = field[3]
    elif third:
        winner = field[6]
    else:
        return False
    return True

def checkDiagonals():
    global winner
    first = field[0] == field[4] == field[8] != "_"
    second = field[6] == field[4] == field[2] != "_"
    
    if first:
        winner = field[0]
    elif second: 
        winner = field[4]
    else:
        return False
    return True

game()