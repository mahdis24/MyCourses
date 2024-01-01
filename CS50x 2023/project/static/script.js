let no_auth_pages = ['register', 'login']
let has_auth_pages = ['orders', 'insert', 'clients', 'onboarding', 'history']

document.addEventListener('DOMContentLoaded', function() {
    let navItems = document.querySelectorAll('a.nav-link')

    let pageName = window.location.pathname.slice(1).split(".")[0]

    let index = no_auth_pages.indexOf(pageName) != -1 ? no_auth_pages.indexOf(pageName) : has_auth_pages.indexOf(pageName) != -1 ? has_auth_pages.indexOf(pageName) : null

    if (index != null) {
        navItems[index].classList.add('is-active')
    }
})
