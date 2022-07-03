module.exports = {
  mode: 'development',
  entry: './_build/default/client/index.bs.js',
  devServer: {
    proxy: {
      '/todos': 'http://localhost:3000'
    },
  },
};
