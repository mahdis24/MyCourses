let pages = ['index', 'about-me', 'skills', 'contact-me']

document.addEventListener('DOMContentLoaded', function() {
    let navItems = document.querySelectorAll('a.nav-link')

    let pageName = window.location.pathname.slice(1).split(".")[0]

    navItems[pages.indexOf(pageName)].classList.add('is-active')
})