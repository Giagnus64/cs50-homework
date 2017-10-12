from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp


from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
     #get username and current cash from logged user
     userrows= db.execute("SELECT username, cash FROM users WHERE id = :id", id=session["user_id"])
     uname = userrows[0]["username"]
     currcash = userrows[0]["cash"]
     #get stock info from user's portfolio
     stockrows = db.execute("SELECT * FROM portfolio WHERE username = :username", username=uname)
     #establish arrays to sort stock price and value logic into
     stockprices = []
     stockvalues = []
     stocksum = 0
     for stock in stockrows:
        #lookup stocks
        tmpstock = lookup(stock["stocksymbol"])
        #append price to proper array
        price = usd(tmpstock.get("price"))
        stockprices.append(price)
        #calculate stock value according to price
        curval = (tmpstock.get("price") * stock["qtyofshares"])
        #append value to proper array
        stockvalues.append(usd(curval))
        #calculate sum of all stocks value
        stocksum = stocksum + curval
        #calculate user's total with stocks and current cash
     usertotal = usd(stocksum + currcash)

     return render_template("index.html", currentcash = usd(currcash), stockrows = stockrows, stockprices = stockprices, stockvalues=stockvalues, usertotal =usertotal)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # if the form is empty
        if not request.form.get("Stock Symbol"):
            return apology("must provide stock symbol")
        # if form is empty or not a positive integer
        if not request.form.get("numofshares") or request.form.get != int or int(request.form.get("numofshares")) <= 0:
            return apology("must insert a positive number of shares")

        # lookup stock using given functions
        else:
            stock = lookup(request.form.get("Stock Symbol"))
            #check if stock symbol is invalid
            if stock == None:
                return apology("Could not retrieve stock data")
            #if stock symbol is valid
            else:
                # get users amount of cash
                rows = db.execute("SELECT username, cash FROM users WHERE id = :id", id=session["user_id"])
                # calculate purchase
                price = stock.get("price")
                purchase = (price * int(request.form.get("numofshares")))
                #check for users cash, return apology if cannot afford
                if (float(rows[0]["cash"]) < purchase):
                    return apology("Not enough cash!")
                #add stock to portfolio
                else:
                    #get users username to place in new table
                    #name = db.execute("SELECT username FROM users WHERE id =")
                    newcash = float(rows[0]["cash"] - purchase)
                    #add stock purchase information to new table
                    newp = db.execute("INSERT INTO purchases (username, stocksymbol, stockprice, qtyofshares, total, stockname) VALUES (:username, :stocksymbol, :stockprice, :qtyofshares, :total, :stockname)", username=rows[0]["username"], stocksymbol=stock.get("symbol"), stockprice =price, qtyofshares=request.form.get("numofshares"), total=purchase, stockname=stock.get("name"))
                    #update users cash
                    db.execute("UPDATE users SET cash = :new WHERE id = :id", new=newcash, id=session["user_id"])
                    #select user shares from portfolio
                    userport=db.execute("SELECT stocksymbol, qtyofshares FROM portfolio WHERE username = :username AND stocksymbol = :stocksymbol", username=rows[0]["username"], stocksymbol=stock.get("symbol"))

                    #make new row for portfolio if none, otherwise update portfolio
                    if not userport :
                        newport = db.execute("INSERT INTO portfolio (username, stocksymbol, qtyofshares, currentvalue, stockname) VALUES (:username, :stocksymbol, :qtyofshares, :currentvalue, :stockname)", username=rows[0]["username"], stocksymbol=stock.get("symbol"), qtyofshares=request.form.get("numofshares"), currentvalue=purchase, stockname=stock.get("name"))

                    else:
                        db.execute("UPDATE portfolio SET qtyofshares= qtyofshares + :newshares, currentvalue = currentvalue + :newpurchase WHERE username = :username AND stocksymbol = :stocksymbol",newshares=request.form.get("numofshares"), newpurchase=purchase, username=rows[0]["username"], stocksymbol=stock.get("symbol"))

                    #return to index
                    return redirect(url_for("index"))
                    #define unique indexes


    else:
        return render_template ("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    #establish current user's username to parse through data
    userrows=db.execute("SELECT username FROM users WHERE id = :id", id=session["user_id"])
    uname = userrows[0]["username"]
    #call for history of users purchases
    buyhistory = db.execute("SELECT * FROM purchases WHERE username = :username", username=uname)
    #call for history of users sales
    sellhistory = db.execute("SELECT * FROM sales WHERE username = :username", username=uname)
    return render_template("history.html", buyhistory = buyhistory, sellhistory = sellhistory)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # if the form is empty
        if not request.form.get("Stock Symbol"):
            return apology("must provide stock symbol")
        # lookup stock using given functions
        else:
            stock = lookup(request.form.get("Stock Symbol"))
            #check if stock symbol is invalid
            if stock == None:
                return apology("Could not retrieve stock data")
            #if stock symbol is valid
            else:
                return render_template("quoted.html", name=stock.get("name"), symbol=stock.get("symbol"), price=usd(stock.get("price")))
    # if user came to page via GET request
    else:
        return render_template ("quote.html")



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # ensure confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must confirm password")

        # ensure password and confirmation match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match")

        #insert username and hash into database
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=request.form.get("username"), hash=pwd_context.hash(request.form.get("password")))

        #ensure username does not already exist
        if not result:
            return apology("username already exists")

        # remember which user has logged in
        session["user_id"] = result


        # redirect user to home page
        return redirect(url_for("index"))


    #if user reaches route via get, render registration page
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # if the form is empty
        if not request.form.get("Stock Symbol"):
            return apology("must provide stock symbol")
        # if form is empty or not a positive integer
        if not request.form.get("numofshares") or request.form.get != int or int(request.form.get("numofshares")) <= 0:
            return apology("must insert a positive number of shares")

        # lookup stock using given functions
        else:
            stock = lookup(request.form.get("Stock Symbol"))
            #check if stock symbol is invalid
            if stock == None:
                return apology("Could not retrieve stock data")
            #if stock symbol is valid
            else:
                #get users username and cash
                userrows= db.execute("SELECT username, cash FROM users WHERE id = :id", id=session["user_id"])
                uname = userrows[0]["username"]
                currcash = userrows[0]["cash"]
                #get users current number of stocks
                stockrows = db.execute("SELECT * FROM portfolio WHERE username = :username AND stocksymbol= :stocksymbol", username=uname, stocksymbol=stock.get("symbol"))
                #calculate amount user wants to sell
                sellamount = (stock.get("price")) * (int(request.form.get("numofshares")))
                #if user does not have enough shares
                if (int(stockrows[0]["qtyofshares"])) < (int(request.form.get("numofshares"))):
                    return apology("You don't have enough shares!")
                #if sell is possible
                else:
                    #calculate new cash
                    newcash = float(currcash + sellamount)
                    #add stock purchase information to new table
                    newp = db.execute("INSERT INTO sales (username, stocksymbol, stockprice, qtyofshares, selltotal, stockname) VALUES (:username, :stocksymbol, :stockprice, :qtyofshares, :selltotal, :stockname)", username=uname, stocksymbol=stock.get("symbol"), stockprice = stock.get("price"), qtyofshares=request.form.get("numofshares"), selltotal=sellamount, stockname=stock.get("name"))
                    #update users cash
                    db.execute("UPDATE users SET cash = :new WHERE id = :id", new=newcash, id=session["user_id"])
                    #select user shares from portfolio
                    userport=db.execute("SELECT stocksymbol, qtyofshares FROM portfolio WHERE username = :username AND stocksymbol = :stocksymbol", username=uname, stocksymbol=stock.get("symbol"))
                    #update user portfolio
                    db.execute("UPDATE portfolio SET qtyofshares= qtyofshares - :newshares WHERE username = :username AND stocksymbol = :stocksymbol",newshares=request.form.get("numofshares"), username=uname, stocksymbol=stock.get("symbol"))
                    portcheck = db.execute("SELECT qtyofshares FROM portfolio WHERE username= :username AND stocksymbol = :stocksymbol", username=uname, stocksymbol=stock.get("symbol"))
                    #if user sells all shares, delete row in table to clean up index portfolio, then return user to index page
                    if portcheck[0]["qtyofshares"] == 0 :
                        db.execute("DELETE FROM portfolio WHERE stocksymbol= :stocksymbol", stocksymbol=stock.get("symbol"))
                        return redirect(url_for("index"))
                    #return user to indexpage
                    else:
                        return redirect(url_for("index"))


    #Render html template for sell if user sends GET request
    else:
        return render_template("sell.html")

@app.route("/pwdchange", methods=["GET", "POST"])
@login_required
def pwdchange():
    """Change User Password"""

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        elif not request.form.get("newpassword"):
            return apology("must provide new password")

        elif not request.form.get("newconfirmation"):
            return apology("must provide confirmation")

        elif request.form.get("newpassword") != request.form.get("newconfirmation"):
            return apology("new password and new confirmation do not match!")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        else:
            db.execute("UPDATE users SET hash = :hash WHERE username = :username", hash=pwd_context.hash(request.form.get("newpassword")), username=request.form.get("username"))

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("pwdchange.html")