import esbuild from 'esbuild';
import { htmlPlugin } from '@craftamap/esbuild-plugin-html';

const options = {
    entryPoints: ['_build/default/client/index.bs.js'],
    bundle: true,
    minify: true,
    metafile: true, // needs to be set
    outdir: 'dist/', // needs to be set
    plugins: [
        htmlPlugin({
            files: [
                {
                    entryPoints: [
                        '_build/default/client/index.bs.js',
                    ],
                    filename: 'index.html',
                    htmlTemplate: 'public/index.html'
                },
            ]
        })
    ]
}

esbuild.build(options).catch(() => process.exit(1))
