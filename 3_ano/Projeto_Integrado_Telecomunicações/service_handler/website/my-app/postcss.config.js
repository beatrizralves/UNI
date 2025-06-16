export default {
  plugins: {
    tailwindcss: {},
    'tailwindcss/nesting': 'postcss-nesting',
    autoprefixer: {},
    'postcss-preset-env': {
      features: { 'nesting-rules': false },
    },
  },
}
