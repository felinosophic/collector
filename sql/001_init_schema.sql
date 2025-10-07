--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: vector; Type: EXTENSION; Schema: -; Owner: -
--

CREATE EXTENSION IF NOT EXISTS vector WITH SCHEMA public;


--
-- Name: EXTENSION vector; Type: COMMENT; Schema: -; Owner: -
--

COMMENT ON EXTENSION vector IS 'vector data type and ivfflat and hnsw access methods';


SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: articles; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.articles (
    id integer NOT NULL,
    title text NOT NULL,
    url text NOT NULL,
    summary text,
    content text,
    published_date text,
    source text NOT NULL,
    processing_state character varying(20) DEFAULT 'unprocessed'::character varying,
    embedding public.vector(1024),
    relevance_rating character varying(20),
    relevance_reason text,
    duplicate_of_id integer,
    fetched_at timestamp with time zone DEFAULT now(),
    CONSTRAINT articles_processing_state_check CHECK (((processing_state)::text = ANY ((ARRAY['unprocessed'::character varying, 'embedded'::character varying, 'classified'::character varying])::text[]))),
    CONSTRAINT articles_relevance_rating_check CHECK (((relevance_rating)::text = ANY ((ARRAY['very_unlikely'::character varying, 'unlikely'::character varying, 'likely'::character varying, 'very_likely'::character varying, 'unsure'::character varying])::text[])))
);


--
-- Name: articles_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.articles_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: articles_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.articles_id_seq OWNED BY public.articles.id;


--
-- Name: sources; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.sources (
    id integer NOT NULL,
    url text NOT NULL,
    name text,
    kind text DEFAULT 'rss'::text NOT NULL,
    name_last_seen_at timestamp with time zone,
    CONSTRAINT sources_kind_check CHECK ((kind = 'rss'::text))
);


--
-- Name: sources_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.sources_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: sources_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.sources_id_seq OWNED BY public.sources.id;


--
-- Name: user_articles; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.user_articles (
    user_id integer NOT NULL,
    article_id integer NOT NULL,
    is_read boolean DEFAULT false NOT NULL,
    is_starred boolean DEFAULT false NOT NULL,
    is_liked text,
    relevance text,
    relevance_reason text,
    scored_at timestamp with time zone,
    model_version text,
    added_at timestamp with time zone DEFAULT now() NOT NULL,
    CONSTRAINT user_articles_is_liked_check CHECK (((is_liked IS NULL) OR (is_liked = ANY (ARRAY['like'::text, 'dislike'::text])))),
    CONSTRAINT user_articles_relevance_check CHECK ((relevance = ANY (ARRAY['very unlikely'::text, 'unlikely'::text, 'likely'::text, 'very likely'::text, 'not sure'::text])))
);


--
-- Name: user_source_subscriptions; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.user_source_subscriptions (
    user_id integer NOT NULL,
    source_id integer NOT NULL,
    source_alias text NOT NULL,
    created_at timestamp with time zone DEFAULT now() NOT NULL,
    last_renamed_at timestamp with time zone DEFAULT now(),
    CONSTRAINT user_source_subscriptions_source_alias_check CHECK (((length(btrim(source_alias)) >= 1) AND (length(btrim(source_alias)) <= 120)))
);


--
-- Name: users; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.users (
    id integer NOT NULL,
    name character varying(255) NOT NULL,
    email character varying(255)
);


--
-- Name: users_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.users_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: users_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.users_id_seq OWNED BY public.users.id;


--
-- Name: articles id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.articles ALTER COLUMN id SET DEFAULT nextval('public.articles_id_seq'::regclass);


--
-- Name: sources id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.sources ALTER COLUMN id SET DEFAULT nextval('public.sources_id_seq'::regclass);


--
-- Name: users id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.users ALTER COLUMN id SET DEFAULT nextval('public.users_id_seq'::regclass);


--
-- Name: articles articles_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.articles
    ADD CONSTRAINT articles_pkey PRIMARY KEY (id);


--
-- Name: articles articles_url_key; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.articles
    ADD CONSTRAINT articles_url_key UNIQUE (url);


--
-- Name: sources sources_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.sources
    ADD CONSTRAINT sources_pkey PRIMARY KEY (id);


--
-- Name: sources sources_url_key; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.sources
    ADD CONSTRAINT sources_url_key UNIQUE (url);


--
-- Name: user_articles user_articles_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.user_articles
    ADD CONSTRAINT user_articles_pkey PRIMARY KEY (user_id, article_id);


--
-- Name: user_source_subscriptions user_source_subscriptions_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.user_source_subscriptions
    ADD CONSTRAINT user_source_subscriptions_pkey PRIMARY KEY (user_id, source_id);


--
-- Name: users users_name_key; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_name_key UNIQUE (name);


--
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (id);


--
-- Name: idx_processing_state; Type: INDEX; Schema: public; Owner: -
--

CREATE INDEX idx_processing_state ON public.articles USING btree (processing_state);


--
-- Name: idx_url; Type: INDEX; Schema: public; Owner: -
--

CREATE INDEX idx_url ON public.articles USING btree (url);


--
-- Name: articles articles_duplicate_of_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.articles
    ADD CONSTRAINT articles_duplicate_of_id_fkey FOREIGN KEY (duplicate_of_id) REFERENCES public.articles(id);


--
-- Name: user_articles user_articles_article_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.user_articles
    ADD CONSTRAINT user_articles_article_id_fkey FOREIGN KEY (article_id) REFERENCES public.articles(id) ON DELETE CASCADE;


--
-- Name: user_articles user_articles_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.user_articles
    ADD CONSTRAINT user_articles_user_id_fkey FOREIGN KEY (user_id) REFERENCES public.users(id) ON DELETE CASCADE;


--
-- Name: user_source_subscriptions user_source_subscriptions_source_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.user_source_subscriptions
    ADD CONSTRAINT user_source_subscriptions_source_id_fkey FOREIGN KEY (source_id) REFERENCES public.sources(id) ON DELETE CASCADE;


--
-- Name: user_source_subscriptions user_source_subscriptions_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.user_source_subscriptions
    ADD CONSTRAINT user_source_subscriptions_user_id_fkey FOREIGN KEY (user_id) REFERENCES public.users(id) ON DELETE CASCADE;


--
-- PostgreSQL database dump complete
--

