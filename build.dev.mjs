import { build, serve } from 'esbuild';
import { createServer, request } from 'http';
import { htmlPlugin } from '@craftamap/esbuild-plugin-html';

const clients = [];
const entryFile = '_build/default/client/index.bs.js';
const buildDir = '_build/dev';
const htmlTempate = 'public/index.html';

build({
    entryPoints: [entryFile],
    bundle: true,

    // These must be set for htmlPlugin
    outdir: buildDir,
    metafile: true,

    // When the page first loads, this will open an event stream back to the client
    banner: { js: ' (() => new EventSource("/esbuild").onmessage = () => location.reload())();' },
    watch: {
      onRebuild(error, result) {
        // On every rebuild, send a update message via event stream
        clients.forEach((res) => res.write('data: update\n\n'))
        clients.length = 0
        console.log(error ? error : '...')
      },
    },
    plugins: [
        htmlPlugin({
            files: [
                {
                    entryPoints: [entryFile],
                    filename: 'index.html',
                    htmlTemplate: htmlTempate
                },
            ]
        })
    ]
  })
  .catch(() => process.exit(1))

// Create a serve server to port 8000
serve({ servedir: buildDir }, {}).then(() => {
  // Then create a new server to 3000
  createServer((req, res) => {
    const { url, method, headers } = req
    // If esbuild, open a new event stream to the client
    if (req.url === '/esbuild')
      return clients.push(
        res.writeHead(200, {
          'Content-Type': 'text/event-stream',
          'Cache-Control': 'no-cache',
          Connection: 'keep-alive',
        })
      )
    // Otherwise just pipe the request to the serve server
    const path = ~url.split('/').pop().indexOf('.') ? url : `/index.html` //for PWA with router
    req.pipe(
      request({ hostname: '0.0.0.0', port: 8000, path, method, headers }, (prxRes) => {
        res.writeHead(prxRes.statusCode, prxRes.headers)
        prxRes.pipe(res, { end: true })
      }),
      { end: true }
    )
  }).listen(3000)
})
