# Eagolf Vendor

#### Video Demo: https://youtu.be/B4thTTd7KB8?feature=shared

#### Description:

### Overview:
Eagolf Vendor is a replica of an inhouse Inventory Management System.
The main purpose of this creation is to centralize and ease small vendor/business owners having to manually track client's subscription status on paper/excelsheet.
For this purpose, you are registered in this panel as a business owner
You have your unique username and password
You can define different clients for yourself and view their list
Create an order for each client, edit the order if needed and see the changes of each order in the history section

This software has the functions as below:
1. Inserting new clients to the list
2. Displaying whole client list
3. Inserting client's order
4. Editing client's order
5. Monitor and track client's order status
6. Register New User Account

Below are the detailed technical aspects of the functionality:

### Nav Bar Items

- ### Register
    - #### Register Page
        Register new account, with username and password (username should be unique)

- ### Login
    - #### Login Page
        Login with the account credentials that was created from Register

- ### Logout
    Logout user and redirect to login page

- ### Account
    - Displays the account username
    - Change password function
        - #### Change Password Page
            Insert old password, new password and retype new password for correctness

- ### LOGO
    - #### Home Page
        - View of all client's order sorted by days left to expiration on "Expired in" column. (ascending order)
        - Edit Function
            - #### Edit Page
                - Displays the selected order information to edit and update
                - Rules according to to **Insert Page**


- ### Insert
    - #### Inster Page
        - Insert and store client's order into the system for tracking
        - All fields are required to be filled
        - S/N (Serial Number) is a unique number generated with Luhn's Algorithm (Algorithm that is used for generating bank credit card numbers)
        - "Expiration Date" MUST be a future date
        - "Client" dropdown list display from client list

- ### Clients
    - #### Clients Page
        Displays all clients in a list from database


- ### Onboarding
    - #### Onboarding Page
        - Insert and store client into the system
        - All fields are required to be filled


- ### History
    - #### History Page
        - Displays the list of all client's orders
        - Able to identify who created the entry
