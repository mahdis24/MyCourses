import math
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import *
from helpers import apology, login_required, luhn, COUNTRIES


# Configure application
app = Flask(__name__)


# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)


# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///vendor.db")


client_db = db.execute("SELECT name FROM client GROUP BY name;")

# Rows Of Table
ROWS_PER_PAGE = 10


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# To display username on every HTML of navbar
@app.context_processor
def inject_user():
    user_id = session.get("user_id")
    if user_id:
        user = db.execute("SELECT username FROM users WHERE id = ?", user_id)
        if user:
            return {"username": user[0]["username"]}
    return {}


@app.route("/")
def index():
    return redirect("/orders")


# Main Page mainly to keep track of contract expiration
@app.route("/orders")
@login_required
def orders():
    # To display x rows per page
    per_page = ROWS_PER_PAGE
    total_rows = db.execute("SELECT COUNT(*) FROM tracking")
    total_pages = math.ceil(total_rows[0]["COUNT(*)"] / per_page)

    page = request.args.get("page", 1, type=int)
    offset = (page - 1) * per_page

    transactions = db.execute(
        "SELECT id, serial_num, Device, ticket_ref, subs_type, expense_type, exp_date, client, addendum, created_at FROM tracking LIMIT (?) OFFSET (?);",
        per_page,
        offset,
    )

    transactions_length = db.execute("SELECT COUNT(*) as total FROM tracking;")[0][
        "total"
    ]

    # Format date from YYYYMMDD extracted from database to YYYY-MM-DD
    for row in transactions:
        exp_date = row["exp_date"]

        year = exp_date // 10000
        month = (exp_date // 100) % 100
        day = exp_date % 100

        date_obj = date(year, month, day)
        row["expiry"] = date_obj

        # calculate the number of days left to expiration
        expiration = date_obj - date.today()
        row["expiration"] = expiration.days

    transactions = sorted(transactions, key=lambda row: row["expiration"])

    return render_template(
        "orders.html",
        database=transactions,
        page=page,
        total_pages=total_pages,
        total_items=transactions_length,
        items_per_page=ROWS_PER_PAGE,
    )


@app.route("/insert", methods=["GET", "POST"])
@login_required
def insert():
    user_id = session["user_id"]

    # Randomly generate unique Serial Number
    serial = luhn()
    sn_duplicate = db.execute("SELECT serial_num from tracking;")
    serial_list = [row["serial_num"] for row in sn_duplicate]
    while serial in serial_list:
        serial = luhn()

    if request.method == "POST":
        device = request.form.get("device")
        ref = request.form.get("ref")
        subtype = request.form.get("subtype")
        exptype = request.form.get("exptype")
        expiry = request.form.get("expiry")
        client = request.form.get("client")
        add = request.form.get("add")

        if not device:
            return apology("Must provide Device Model.", 400)

        if not ref:
            return apology("Must provide Ticket Reference.", 400)

        if not subtype:
            return apology("Must provide Subscription Type.", 400)

        if not exptype:
            return apology("Must provide Expenditure Type.", 400)

        if not expiry:
            return apology("Must provide Expiration Date.", 400)
        else:
            date_obj = datetime.strptime(expiry, "%Y-%m-%d")

            if date_obj <= datetime.today():
                flash("Expiry date not valid!")
                return redirect("/insert")

            formatted_date = date_obj.strftime("%Y%m%d")

        if not client:
            return apology("Must provide client name.", 400)

        if not add:
            return apology("Must provide Addendum.", 400)

        db.execute(
            "INSERT INTO tracking (serial_num, Device, ticket_ref, subs_type, expense_type, exp_date, client, Addendum, user_id) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)",
            serial,
            device,
            ref,
            subtype,
            exptype,
            int(formatted_date),
            client,
            add,
            user_id,
        )

        flash("Insert Successful!")
        return redirect("/orders")

    return render_template("insert.html", serial=serial, client_db=client_db)


@app.route("/edit", methods=["GET", "POST"])
@login_required
def edit():
    user_id = session["user_id"]

    # To retrieve data from database to HTML
    if request.method == "GET":
        try:
            id = request.args.get("id")
            info = db.execute("SELECT * FROM tracking WHERE id = (?)", id)
            if info:
                info = info[0]
            else:
                return apology("Not Exist!", 404)

            # Change date format from database to HTML compatible
            db_date = info["exp_date"]
            date_obj = datetime.strptime(str(db_date), "%Y%m%d")
            formatted_date = date_obj.strftime("%Y-%m-%d")

            if info["user_id"] != user_id:
                return apology("This data does not belong to you!", 403)

            return render_template(
                "edit.html",
                info=info,
                client_db=client_db,
                formatted_date=formatted_date,
            )

        except ValueError:
            pass

        return apology("Invalid Request", 403)

    # To edit and update data from HTML to database
    if request.method == "POST":
        id = request.form.get("id")
        # serial = request.form.get("serial")
        device = request.form.get("device")
        ref = request.form.get("ref")
        subtype = request.form.get("subtype")
        exptype = request.form.get("exptype")
        expiry = request.form.get("expiry")
        client = request.form.get("client")
        add = request.form.get("add")

        info = db.execute("SELECT * FROM tracking WHERE id = (?)", id)
        if info:
            info = info[0]
        else:
            return apology("Not Exist!", 404)

        if not device:
            device = info["Device"]

        if not ref:
            ref = info["ticket_ref"]

        if not subtype:
            subtype = info["subs_type"]

        if not exptype:
            exptype = info["expense_type"]

        if not expiry:
            return apology("Must provide Expiration Date.", 400)
        else:
            date_obj = datetime.strptime(expiry, "%Y-%m-%d")

            if date_obj <= datetime.today():
                flash("Expiry date not valid!")
                return redirect("/edit?id=" + id)

            formatted_date = date_obj.strftime("%Y%m%d")

        if not client:
            client = info["client"]

        if not add:
            return apology("Must provide Addendum.", 400)

        db.execute(
            "UPDATE tracking SET Device = ?, ticket_ref = ?, subs_type = ?, expense_type = ?, exp_date = ?, client = ?, Addendum = ?, user_id = ? WHERE id = ?",
            device,
            ref,
            subtype,
            exptype,
            formatted_date,
            client,
            add,
            user_id,
            id,
        )
        flash("Edit Successful!")
        return redirect("/orders")


@app.route("/history")
@login_required
def history():
    # Overview of data input by internal team
    # Only displaying 10 rows per page
    per_page = ROWS_PER_PAGE
    total_rows = db.execute("SELECT COUNT(*) FROM tracking")
    total_pages = math.ceil(total_rows[0]["COUNT(*)"] / per_page)

    page = request.args.get("page", 1, type=int)
    offset = (page - 1) * per_page

    transactions = db.execute(
        "SELECT tracking.id AS tracking_id, * FROM tracking JOIN users on users.id = tracking.user_id;"
    )

    transactions_length = len(transactions)

    # Formatting date of database to be compatible with HTML's
    for row in transactions:
        exp_date = row["exp_date"]

        year = exp_date // 10000
        month = (exp_date // 100) % 100
        day = exp_date % 100

        date_obj = date(year, month, day)
        row["expiry"] = date_obj

        expiration = date_obj - date.today()
        row["expiration"] = expiration.days

    transactions = transactions[offset : offset + per_page]

    return render_template(
        "history.html",
        database=transactions,
        page=page,
        total_pages=total_pages,
        total_items=transactions_length,
        items_per_page=ROWS_PER_PAGE,
    )


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")

        # Ensure username was submitted
        if not username:
            return apology("Must provide username", 400)

        # Ensure password was submitted
        elif not password:
            return apology("Must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], password):
            return apology("Invalid username or password", 404)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/onboarding", methods=["GET", "POST"])
@login_required
def onboarding():
    # Onboarding new client's details
    countries = COUNTRIES

    if request.method == "POST":
        name = request.form.get("client_name")
        contact_no = request.form.get("contact_no")
        address = request.form.get("address")
        country = request.form.get("country")
        city = request.form.get("city_name")
        state = request.form.get("state")
        postal_code = request.form.get("postal_code")

        if not name:
            return apology("Must provide client's name!", 400)

        if not contact_no:
            return apology("Must provide client's contact number!", 400)

        if not address:
            return apology("Must provide client's address!", 400)

        if not country:
            return apology("Must provide client's country!", 400)

        if not city:
            return apology("Must provide client's city!", 400)

        if not state:
            return apology("Must provide client's state!", 400)

        if not postal_code:
            return apology("Must provide client's postal code!", 400)

        db.execute(
            "INSERT INTO client (name, contact_no, address, country, city, state, postal_code) VALUES (?, ?, ?, ?, ?, ?, ?);",
            name,
            contact_no,
            address,
            country,
            city,
            state,
            postal_code,
        )

        flash("Onboard Successfully!")
        return redirect("/clients")

    else:
        return render_template("client_onboarding.html", countries=countries)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    username = request.form.get("username")
    password = request.form.get("password")
    confirmation = request.form.get("confirmation")

    if request.method == "POST":
        # Ensure username was submitted
        if not username:
            return apology("Must provide username", 400)

        # Ensure password was submitted
        elif not password:
            return apology("Must provide password", 400)

        elif password != confirmation:
            return apology("Password do not match", 400)

        # Query database for username
        try:
            db.execute(
                "INSERT INTO users (username, hash) VALUES(?, ?);",
                username,
                generate_password_hash(password),
            )
        except:
            return apology("Username is taken, please try another username", 403)

        flash("Registered Successful!")
        # Redirect user to home page
        return redirect("/login")

    else:
        return render_template("register.html")


@app.route("/clients", methods=["GET", "POST"])
@login_required
def clients():
    # Display all client list
    if request.method == "GET":
        database = db.execute("SELECT * FROM client;")
        return render_template("clients.html", database=database)

    return redirect("/clients")


@app.route("/change_password", methods=["GET", "POST"])
def change_password():
    """Register user"""
    user_id = session["user_id"]
    password = request.form.get("password")
    newpassword = request.form.get("newpassword")
    confirmation = request.form.get("confirmation")

    if request.method == "POST":
        # Ensure username was submitted
        if not password:
            return apology("Must provide password", 400)

        # Ensure password was submitted
        elif not newpassword:
            return apology("Must provide new password", 400)

        elif newpassword != confirmation:
            return apology("Password do not match!", 400)

        rows = db.execute("SELECT * FROM users WHERE id = ?", user_id)

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], password):
            return apology("Password incorrect!", 403)

        # Query database for username
        try:
            db.execute(
                "UPDATE users SET hash = (?) WHERE id = (?);",
                generate_password_hash(newpassword),
                user_id,
            )
        except:
            return apology("Please try again", 400)

        flash("Password change Successful!")
        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("change_password.html")


@app.errorhandler(404)
def page_not_found(e):
    return redirect("/orders")
