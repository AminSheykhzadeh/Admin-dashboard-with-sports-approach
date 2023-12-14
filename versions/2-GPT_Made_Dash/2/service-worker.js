// service-worker.js
const CACHE_NAME = 'dashboard-cache-v1';
const urlsToCache = [
  '/',
  '/styles.css',
  '/script.js',
  '/logo.png',
  // Add URLs for other assets like CSS, JavaScript files, and images
];

self.addEventListener('install', event => {
  event.waitUntil(
    caches.open(CACHE_NAME)
      .then(cache => {
        console.log('Opened cache');
        return cache.addAll(urlsToCache);
      })
  );
});

self.addEventListener('fetch', event => {
  event.respondWith(
    caches.match(event.request)
      .then(response => {
        // Cache hit - return response
        if (response) {
          return response;
        }
        return fetch(event.request);
      }
    )
  );
});
